#!/bin/bash

# Thermal Radiation Analysis System - Master Control Script
# Usage: ./run.sh [command]
# Commands: setup, start, stop, restart, status, test

BACKEND_PORT=8080
FRONTEND_PORT=3000

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

print_header() {
    echo -e "${BLUE}==========================================${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}==========================================${NC}"
}

print_success() {
    echo -e "${GREEN}✅ $1${NC}"
}

print_error() {
    echo -e "${RED}❌ $1${NC}"
}

print_info() {
    echo -e "${YELLOW}ℹ️  $1${NC}"
}

# Setup and build
setup() {
    print_header "Setup & Build"
    
    # Download cpp-httplib if not present
    if [ ! -f "backend/include/httplib.h" ]; then
        echo "Downloading cpp-httplib..."
        curl -L -o backend/include/httplib.h https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h
        if [ $? -eq 0 ]; then
            print_success "cpp-httplib downloaded"
        else
            print_error "Failed to download cpp-httplib"
            exit 1
        fi
    else
        print_success "cpp-httplib already present"
    fi
    
    # Compile the server
    echo "Compiling server..."
    g++ -std=c++17 -O2 -o bin/server backend/server.cpp -I backend -lpthread
    
    if [ $? -eq 0 ]; then
        print_success "Server compiled successfully"
        echo
        print_success "Setup complete! Run './run.sh start' to begin"
    else
        print_error "Failed to compile server"
        exit 1
    fi
}

# Check if server is running on a port
is_running() {
    lsof -Pi :$1 -sTCP:LISTEN -t >/dev/null 2>&1
}

# Get PID of process on port
get_pid() {
    lsof -Pi :$1 -sTCP:LISTEN -t 2>/dev/null
}

# Start backend
start_backend() {
    if [ ! -f "bin/server" ]; then
        print_error "Backend not compiled. Run './run.sh setup' first"
        return 1
    fi
    
    if is_running $BACKEND_PORT; then
        print_success "Backend already running on port $BACKEND_PORT"
    else
        echo "Starting backend server..."
        ./bin/server &
        sleep 2
        if is_running $BACKEND_PORT; then
            print_success "Backend started on http://localhost:$BACKEND_PORT"
        else
            print_error "Failed to start backend"
            return 1
        fi
    fi
}

# Start frontend
start_frontend() {
    if is_running $FRONTEND_PORT; then
        print_success "Frontend already running on port $FRONTEND_PORT"
    else
        echo "Starting frontend server..."
        (cd frontend && python3 -m http.server $FRONTEND_PORT --bind 0.0.0.0) >/dev/null 2>&1 &
        sleep 2
        if is_running $FRONTEND_PORT; then
            print_success "Frontend started on http://localhost:$FRONTEND_PORT"
            echo "   Network access: http://192.168.0.218:$FRONTEND_PORT"
        else
            print_error "Failed to start frontend"
            return 1
        fi
    fi
}

# Start both servers
start() {
    print_header "Starting System"
    
    start_backend
    BACKEND_OK=$?
    
    start_frontend
    FRONTEND_OK=$?
    
    if [ $BACKEND_OK -eq 0 ] && [ $FRONTEND_OK -eq 0 ]; then
        echo
        print_header "✅ System Ready!"
        echo
        echo "🌐 Open your browser:"
        echo "   → ${GREEN}http://localhost:$FRONTEND_PORT${NC} (local)"
        echo "   → ${GREEN}http://192.168.0.218:$FRONTEND_PORT${NC} (network)"
        echo
        echo "📡 Backend API:"
        echo "   → http://localhost:$BACKEND_PORT (local)"
        echo "   → http://192.168.0.218:$BACKEND_PORT (network)"
        echo
        echo "To stop: ./run.sh stop"
        echo
        
        # Auto-open browser
        sleep 1
        open "http://localhost:$FRONTEND_PORT" 2>/dev/null || true
    fi
}

# Stop servers
stop() {
    print_header "Stopping System"
    
    # Stop backend
    BACKEND_PID=$(get_pid $BACKEND_PORT)
    if [ ! -z "$BACKEND_PID" ]; then
        kill $BACKEND_PID 2>/dev/null
        print_success "Backend stopped"
    else
        print_info "Backend not running"
    fi
    
    # Stop frontend
    FRONTEND_PID=$(get_pid $FRONTEND_PORT)
    if [ ! -z "$FRONTEND_PID" ]; then
        kill $FRONTEND_PID 2>/dev/null
        print_success "Frontend stopped"
    else
        print_info "Frontend not running"
    fi
    
    echo
    print_success "All servers stopped"
}

# Restart
restart() {
    print_header "Restarting System"
    stop
    sleep 2
    start
}

# Status
status() {
    print_header "System Status"
    
    echo
    echo "Backend (port $BACKEND_PORT):"
    if is_running $BACKEND_PORT; then
        BACKEND_PID=$(get_pid $BACKEND_PORT)
        print_success "Running (PID: $BACKEND_PID)"
        
        # Test backend
        HEALTH=$(curl -s http://localhost:$BACKEND_PORT/health 2>/dev/null)
        if [ ! -z "$HEALTH" ]; then
            echo "   Response: $HEALTH"
        fi
    else
        print_error "Not running"
    fi
    
    echo
    echo "Frontend (port $FRONTEND_PORT):"
    if is_running $FRONTEND_PORT; then
        FRONTEND_PID=$(get_pid $FRONTEND_PORT)
        print_success "Running (PID: $FRONTEND_PID)"
        echo "   URL: http://localhost:$FRONTEND_PORT"
    else
        print_error "Not running"
    fi
    echo
}

# Test
test_system() {
    print_header "Testing System"
    
    # Test backend health
    echo "Testing backend health..."
    HEALTH=$(curl -s http://localhost:$BACKEND_PORT/health)
    if [ ! -z "$HEALTH" ]; then
        print_success "Backend responding: $HEALTH"
    else
        print_error "Backend not responding"
    fi
    
    # Test backend status
    echo "Testing backend status..."
    STATUS=$(curl -s http://localhost:$BACKEND_PORT/status)
    if [ ! -z "$STATUS" ]; then
        print_success "Status: $STATUS"
    else
        print_error "Status check failed"
    fi
    
    # Test frontend
    echo "Testing frontend..."
    FRONTEND_STATUS=$(curl -s -o /dev/null -w "%{http_code}" http://localhost:$FRONTEND_PORT 2>/dev/null)
    if [ "$FRONTEND_STATUS" = "200" ]; then
        print_success "Frontend responding (HTTP $FRONTEND_STATUS)"
    else
        print_error "Frontend not responding"
    fi
    echo
}

# Show usage
usage() {
    echo "Thermal Radiation Analysis System - Control Script"
    echo
    echo "Usage: ./run.sh [command]"
    echo
    echo "Commands:"
    echo "  setup      Download dependencies and compile"
    echo "  start      Start both servers and open browser"
    echo "  stop       Stop all servers"
    echo "  restart    Restart all servers"
    echo "  status     Check if servers are running"
    echo "  test       Test server endpoints"
    echo "  help       Show this help message"
    echo
    echo "Examples:"
    echo "  ./run.sh setup      # First time setup"
    echo "  ./run.sh start      # Start using the system"
    echo "  ./run.sh status     # Check what's running"
    echo "  ./run.sh stop       # Stop everything"
    echo
}

# Main
case "${1:-}" in
    setup)
        setup
        ;;
    start)
        start
        ;;
    stop)
        stop
        ;;
    restart)
        restart
        ;;
    status)
        status
        ;;
    test)
        test_system
        ;;
    help|--help|-h)
        usage
        ;;
    *)
        if [ -z "$1" ]; then
            usage
        else
            print_error "Unknown command: $1"
            echo
            usage
            exit 1
        fi
        ;;
esac
