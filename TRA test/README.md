# Thermal Radiation Analysis System

A full-stack thermal radiation analysis system with C++ backend and interactive 3D frontend.

## 🚀 Quick Start

### First Time Setup
```bash
./run.sh setup
```

### Start Everything
```bash
./run.sh start
```
This starts both servers and opens your browser automatically!

### Stop Everything
```bash
./run.sh stop
```

### Check Status
```bash
./run.sh status
```

### All Commands
```bash
./run.sh           # Show help
./run.sh setup     # Download dependencies and compile
./run.sh start     # Start both servers
./run.sh stop      # Stop all servers
./run.sh restart   # Restart everything
./run.sh status    # Check what's running
./run.sh test      # Test server endpoints
```

---

## ⚠️ Important: Don't Open as File!

❌ **DON'T** double-click `index.html` to open it  
❌ **DON'T** open via `file://` protocol

✅ **DO** use http://localhost:3000

**Why?** Browsers block HTTP requests from `file://` URLs due to CORS security policies.

---

## 🏗️ Architecture

```
┌─────────────────────────────────────┐
│  Browser: http://localhost:3000     │
│  (Frontend: index.html)             │
└──────────────┬──────────────────────┘
               │
               │ HTTP POST /calculate
               │ (JSON)
               ▼
┌─────────────────────────────────────┐
│  Backend: http://localhost:8080     │
│  (C++ Server: ./server)             │
│  - Ray Tracing                      │
│  - View Factor Calculations         │
│  - Temperature Distribution         │
└─────────────────────────────────────┘
```

---

## 📁 Project Structure

```
.
├── README.md                    # Documentation
├── run.sh                       # Master control script
├── frontend/
│   └── index.html              # Web UI (3D visualization)
├── backend/
│   ├── server.cpp              # C++ backend server
│   ├── calcus.cpp              # Legacy CLI tool (optional)
│   └── include/
│       └── httplib.h           # HTTP library dependency
└── bin/
    └── server                  # Compiled backend binary
```

| Path | Purpose | Required? |
|------|---------|-----------|
| `frontend/index.html` | Frontend UI | ✅ Yes |
| `backend/server.cpp` | Backend source | ✅ Yes |
| `backend/include/httplib.h` | HTTP library | ✅ Yes |
| `bin/server` | Backend binary | ✅ Yes (auto-compiled) |
| `run.sh` | Control script | ✅ Yes |
| `README.md` | Documentation | Recommended |
| `backend/calcus.cpp` | Legacy CLI tool | Optional |

---

## 🔧 Rebuilding

If you modify `server.cpp`:

```bash
# Recompile and restart
./run.sh setup
./run.sh restart
```

---

## 🧪 Testing

```bash
# Test all endpoints
./run.sh test

# Or manually test backend
curl http://localhost:8080/health
curl http://localhost:8080/status
```

---

## 📊 Usage

1. **Open browser**: http://localhost:3000
2. **Create geometry**: Add polygons in 3D space
3. **Set temperatures**: Assign temperatures to emitter surfaces
4. **Add receiver planes**: Define where to measure radiation
5. **Calculate**: Click calculate button
6. **View results**: See heat maps and contours

---

## 🐛 Troubleshooting

### "Failed to fetch" error
- Make sure both servers are running: `./run.sh status`
- Start if needed: `./run.sh start`
- Access via http://localhost:3000 (not file://)
- Check backend: `./run.sh test`

### Port already in use
```bash
# Check what's using the port
lsof -i :8080
lsof -i :3000

# Kill the process
kill <PID>
```

### Server won't compile
```bash
# Check compiler version (needs C++17)
g++ --version

# Install/update if needed (macOS)
xcode-select --install
```

---

## 🔌 Ports

- Frontend: http://localhost:3000
- Backend: http://localhost:8080

To change ports, edit:
- Backend: `backend/server.cpp` (line 844: `svr.listen("0.0.0.0", 8080)`)
- Frontend: `run.sh` (change `FRONTEND_PORT=3000`)
- Config: `frontend/index.html` (line 792: `BACKEND_URL`)

---

## 🌐 Network Deployment

### Current Configuration
The app is configured for company network deployment:
- **Server IP**: 192.168.0.218:8080
- **Backend**: Binds to `0.0.0.0` (accepts connections from any network interface)
- **CORS**: Enabled for all origins

### Access from Company Network
Colleagues can access the app at:
```
http://192.168.0.218:3000
```

### Server Setup
1. On the server machine (192.168.0.218):
   ```bash
   ./run.sh start
   ```

2. Verify the server is accessible:
   ```bash
   curl http://192.168.0.218:8080/health
   ```

3. Colleagues can now open their browsers and navigate to:
   - **Frontend**: http://192.168.0.218:3000
   - **Backend API**: http://192.168.0.218:8080 (used automatically by frontend)

### Firewall Configuration
Ensure the server machine allows incoming connections on ports:
- **Port 3000**: Frontend web server
- **Port 8080**: Backend API server

---

## 📝 License

Same as original project.
