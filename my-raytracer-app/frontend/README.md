# Frontend Deployment Guide

This guide explains how to deploy the frontend to work with your backend API.

## Configuration

The frontend is configured to communicate with the backend through the `CONFIG` object at the top of `index.html`:

```javascript
const CONFIG = {
    // Backend API URL - Change this to your deployed backend URL
    BACKEND_URL: 'http://localhost:5000', // For local development
    // BACKEND_URL: 'https://your-app.onrender.com', // For Render.com deployment
    // BACKEND_URL: 'https://your-domain.com', // For custom domain
    
    // API endpoints
    CALCULATE_ENDPOINT: '/calculate',
    HEALTH_ENDPOINT: '/health',
    STATUS_ENDPOINT: '/status',
    
    // Request settings
    REQUEST_TIMEOUT: 60000, // 60 seconds
    MAX_RETRIES: 3
};
```

## Deployment Options

### 1. Local Development
- Keep `BACKEND_URL: 'http://localhost:5000'`
- Run your backend locally on port 5000
- Open `index.html` in your browser

### 2. Render.com Deployment
- Change `BACKEND_URL` to your Render.com app URL
- Example: `BACKEND_URL: 'https://ray-tracer-api.onrender.com'`
- Deploy the frontend to any static hosting service

### 3. Custom Domain
- Change `BACKEND_URL` to your custom domain
- Example: `BACKEND_URL: 'https://api.yourdomain.com'`

## API Communication

The frontend communicates with the backend through these endpoints:

### POST /calculate
- **Purpose**: Main ray tracing calculation
- **Input**: JSON with receiver_planes, polygons, inert_polygons, num_rays
- **Output**: JSON with success, plane_info, raw_output

### GET /health
- **Purpose**: Health check
- **Output**: JSON with status information

### GET /status
- **Purpose**: Server status and configuration
- **Output**: JSON with detailed server information

## Data Flow

1. **User clicks "Calculate"** → `Pointsinfo()` function is called
2. **Data preparation** → Frontend processes 3D plane data into JSON format
3. **API request** → Sends JSON to backend `/calculate` endpoint
4. **Backend processing** → C++ ray tracer processes the data
5. **Response handling** → Frontend receives temperature values
6. **Contour generation** → Frontend creates D3.js contour visualization
7. **Display** → Shows contour plot in modal

## Error Handling

The frontend includes comprehensive error handling:

- **Connection errors**: Shows backend connection status
- **API errors**: Displays detailed error messages
- **Loading states**: Shows "Calculating..." during processing
- **Timeout handling**: 60-second timeout for requests

## Testing

### Test Backend Connection
Open browser console and check for:
- ✅ Backend is healthy
- Backend status information

### Test Calculation
1. Add some planes to the scene
2. Set plane types (Receiver, Emitter, Inert)
3. Click "Calculate"
4. Check console for API communication logs

## Troubleshooting

### Backend Not Responding
1. Check `CONFIG.BACKEND_URL` is correct
2. Verify backend is running and accessible
3. Check browser console for CORS errors
4. Test backend URL directly in browser

### CORS Issues
If you get CORS errors:
1. Ensure backend has CORS enabled
2. Check that backend URL is correct
3. Verify backend is running on HTTPS if frontend is on HTTPS

### Calculation Errors
1. Check console for detailed error messages
2. Verify JSON data format is correct
3. Check backend logs for processing errors
4. Ensure all required fields are present

## Performance Optimization

### For Large Scenes
- Reduce `num_rays` in the calculation
- Use smaller grid sizes for receiver planes
- Consider upgrading to paid backend hosting

### For Slow Networks
- Increase `REQUEST_TIMEOUT` in config
- Add retry logic for failed requests
- Show progress indicators for long calculations

## Security Considerations

- Backend URL should use HTTPS in production
- Validate all input data before sending to backend
- Implement rate limiting for API requests
- Use environment variables for sensitive configuration

## Development Tips

1. **Enable console logging** to see API communication
2. **Test with small datasets** first
3. **Use browser dev tools** to inspect network requests
4. **Check backend logs** for processing information
5. **Validate JSON format** before sending to backend