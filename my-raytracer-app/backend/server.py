#!/usr/bin/env python3
"""
Ray Tracer Web Server for Render.com
A simple Flask-based web server that provides a REST API for the C++ ray tracing engine.
"""

import os
import sys
import json
import subprocess
import tempfile
import logging
from flask import Flask, request, jsonify
from flask_cors import CORS
import traceback

# Configure logging
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(levelname)s - %(message)s'
)
logger = logging.getLogger(__name__)

# Initialize Flask app
app = Flask(__name__)
CORS(app)  # Enable CORS for all routes

# Configuration
RAY_TRACER_BINARY = './calcus'
PORT = int(os.environ.get('PORT', 10000))
MAX_REQUEST_SIZE = 5 * 1024 * 1024  # 5MB for free tier

logger.info(f"Ray tracer binary: {RAY_TRACER_BINARY}")
logger.info(f"Server port: {PORT}")
logger.info(f"Max request size: {MAX_REQUEST_SIZE} bytes")


def validate_json_input(data):
    """Validate the input JSON structure."""
    required_fields = ['receiver_planes', 'polygons']
    
    for field in required_fields:
        if field not in data:
            return False, f"Missing required field: {field}"
    
    # Validate receiver_planes structure
    if not isinstance(data['receiver_planes'], dict):
        return False, "receiver_planes must be an object"
    
    # Validate polygons structure
    if not isinstance(data['polygons'], list):
        return False, "polygons must be an array"
    
    return True, "Valid"


def run_ray_tracer(json_data):
    """Execute the C++ ray tracer with the provided JSON data."""
    try:
        # Create a temporary file for JSON input
        with tempfile.NamedTemporaryFile(mode='w', suffix='.json', delete=False) as temp_file:
            json.dump(json_data, temp_file, indent=2)
            temp_file_path = temp_file.name
        
        # Run the C++ binary with the JSON file
        result = subprocess.run(
            [RAY_TRACER_BINARY, temp_file_path],
            capture_output=True,
            text=True,
            timeout=60,  # 1 minute timeout for free tier
            cwd='/app'
        )
        
        # Clean up temporary file
        os.unlink(temp_file_path)
        
        if result.returncode != 0:
            logger.error(f"Ray tracer failed with return code {result.returncode}")
            logger.error(f"STDERR: {result.stderr}")
            return None, f"Ray tracer execution failed: {result.stderr}"
        
        return result.stdout, None
        
    except subprocess.TimeoutExpired:
        logger.error("Ray tracer execution timed out")
        return None, "Ray tracer execution timed out"
    except Exception as e:
        logger.error(f"Error running ray tracer: {str(e)}")
        return None, f"Error running ray tracer: {str(e)}"


@app.route('/health', methods=['GET'])
def health_check():
    """Health check endpoint."""
    return jsonify({
        'status': 'healthy',
        'service': 'ray-tracer-api',
        'version': '1.0.0'
    })


@app.route('/calculate', methods=['POST'])
def calculate():
    """Calculate radiation contours using the C++ ray tracer."""
    try:
        # Check content length
        if request.content_length and request.content_length > MAX_REQUEST_SIZE:
            return jsonify({
                'error': f'Request too large. Maximum size: {MAX_REQUEST_SIZE} bytes'
            }), 413
        
        # Get JSON data
        try:
            data = request.get_json()
        except Exception as e:
            return jsonify({
                'error': f'Invalid JSON: {str(e)}'
            }), 400
        
        if not data:
            return jsonify({
                'error': 'No JSON data provided'
            }), 400
        
        # Validate input
        is_valid, error_msg = validate_json_input(data)
        if not is_valid:
            return jsonify({
                'error': f'Invalid input: {error_msg}'
            }), 400
        
        logger.info("Processing calculation request")
        
        # Run the ray tracer
        result, error = run_ray_tracer(data)
        
        if error:
            logger.error(f"Ray tracer error: {error}")
            return jsonify({
                'error': error
            }), 500
        
        # Parse the result
        try:
            # The C++ binary returns a specific format
            lines = result.strip().split('\n')
            
            # Extract plane information
            plane_info = {}
            for line in lines:
                if line.startswith('Plane: '):
                    plane_info['name'] = line[7:]
                elif line.startswith('Width: '):
                    plane_info['width'] = float(line[7:])
                elif line.startswith('Height: '):
                    plane_info['height'] = float(line[8:])
                elif line and not line.startswith(('Plane:', 'Width:', 'Height:')):
                    # This should be the temperature values
                    temperature_values = [float(x) for x in line.split()]
                    plane_info['values'] = temperature_values
            
            # Create response
            response = {
                'success': True,
                'plane_info': plane_info,
                'raw_output': result
            }
            
            logger.info("Calculation completed successfully")
            return jsonify(response)
            
        except Exception as e:
            logger.error(f"Error parsing ray tracer output: {str(e)}")
            return jsonify({
                'error': f'Error parsing results: {str(e)}',
                'raw_output': result
            }), 500
        
    except Exception as e:
        logger.error(f"Unexpected error in calculate: {str(e)}")
        logger.error(traceback.format_exc())
        return jsonify({
            'error': f'Internal server error: {str(e)}'
        }), 500


@app.route('/status', methods=['GET'])
def get_status():
    """Get server status and configuration."""
    return jsonify({
        'status': 'running',
        'ray_tracer_binary': RAY_TRACER_BINARY,
        'binary_exists': os.path.exists(RAY_TRACER_BINARY),
        'binary_executable': os.access(RAY_TRACER_BINARY, os.X_OK),
        'max_request_size': MAX_REQUEST_SIZE,
        'port': PORT
    })


@app.route('/test', methods=['POST'])
def test_endpoint():
    """Test endpoint with sample data."""
    sample_data = {
        "receiver_planes": {
            "plane1": {
                "width": 2.0,
                "height": 2.0,
                "points": [
                    {
                        "origin": [0.0, 0.0, 0.0],
                        "normal": [0.0, 1.0, 0.0]
                    }
                ]
            }
        },
        "polygons": [
            {
                "polygon": [
                    [0.0, 1.0, 0.0],
                    [1.0, 1.0, 0.0],
                    [1.0, 1.0, 1.0],
                    [0.0, 1.0, 1.0]
                ],
                "temperature": 100.0
            }
        ],
        "num_rays": 1000
    }
    
    result, error = run_ray_tracer(sample_data)
    
    if error:
        return jsonify({
            'error': error,
            'test_data': sample_data
        }), 500
    
    return jsonify({
        'success': True,
        'test_data': sample_data,
        'result': result
    })


@app.errorhandler(404)
def not_found(error):
    """Handle 404 errors."""
    return jsonify({
        'error': 'Endpoint not found',
        'available_endpoints': [
            'GET /health',
            'GET /status',
            'POST /calculate',
            'POST /test'
        ]
    }), 404


@app.errorhandler(405)
def method_not_allowed(error):
    """Handle 405 errors."""
    return jsonify({
        'error': 'Method not allowed'
    }), 405


@app.errorhandler(500)
def internal_error(error):
    """Handle 500 errors."""
    logger.error(f"Internal server error: {str(error)}")
    return jsonify({
        'error': 'Internal server error'
    }), 500


if __name__ == '__main__':
    logger.info(f"Starting Ray Tracer Web Server on port {PORT}")
    logger.info(f"Ray tracer binary: {RAY_TRACER_BINARY}")
    
    # Run the Flask app
    app.run(
        host='0.0.0.0',
        port=PORT,
        debug=False,
        threaded=True
    )