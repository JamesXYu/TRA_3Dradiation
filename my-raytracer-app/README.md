# Ray Tracer Web Application - Render.com Deployment Guide

A complete web deployment setup for a C++ ray tracing application with a web frontend, optimized for Render.com's free tier.

## 📋 Prerequisites

- [Render.com account](https://render.com) (free tier available)
- Git repository (GitHub, GitLab, or Bitbucket)
- Your code pushed to the repository

## 🚀 Deployment Steps

### Step 1: Deploy Backend as Web Service

#### 1.1 Create New Web Service
1. Log in to [Render.com](https://render.com)
2. Click **"New +"** → **"Web Service"**
3. Connect your Git repository
4. Select the repository containing your code

#### 1.2 Configure Backend Service
Fill in the following settings:

**Basic Settings:**
- **Name**: `ray-tracer-backend`
- **Environment**: `Docker`
- **Region**: `Oregon` (or your preferred region)
- **Branch**: `main`
- **Root Directory**: `backend`

**Build & Deploy:**
- **Dockerfile Path**: `./Dockerfile`
- **Docker Context**: `./`
- **Build Command**: *(leave empty - handled by Dockerfile)*
- **Start Command**: *(leave empty - handled by Dockerfile)*

**Environment Variables:**
```
PORT=10000
RAY_TRACER_BINARY=./calcus
MAX_REQUEST_SIZE=5242880
```

**Health Check:**
- **Health Check Path**: `/health`

**Plan:**
- **Plan**: `Free`

#### 1.3 Deploy Backend
1. Click **"Create Web Service"**
2. Wait for the build to complete (5-10 minutes)
3. Note the service URL (e.g., `https://ray-tracer-backend.onrender.com`)

### Step 2: Deploy Frontend as Static Site

#### 2.1 Create New Static Site
1. In Render.com dashboard, click **"New +"** → **"Static Site"**
2. Connect the same Git repository
3. Select the repository

#### 2.2 Configure Frontend Service
Fill in the following settings:

**Basic Settings:**
- **Name**: `ray-tracer-frontend`
- **Branch**: `main`
- **Root Directory**: `frontend`

**Build & Deploy:**
- **Build Command**: *(leave empty - static files)*
- **Publish Directory**: `frontend`

**Plan:**
- **Plan**: `Free`

#### 2.3 Deploy Frontend
1. Click **"Create Static Site"**
2. Wait for deployment to complete (2-3 minutes)
3. Note the site URL (e.g., `https://ray-tracer-frontend.onrender.com`)

### Step 3: Update Frontend Backend URL

#### 3.1 Locate Configuration
1. Go to your Git repository
2. Navigate to `frontend/index.html`
3. Find the `CONFIG` object around line 660:

```javascript
const CONFIG = {
    // Backend API URL - Change this to your deployed backend URL
    BACKEND_URL: 'http://localhost:5000', // For local development
    // BACKEND_URL: 'https://your-app.onrender.com', // For Render.com deployment
    // BACKEND_URL: 'https://your-domain.com', // For custom domain
    // ... rest of config
};
```

#### 3.2 Update Backend URL
1. Replace the `BACKEND_URL` with your deployed backend URL:
```javascript
const CONFIG = {
    BACKEND_URL: 'https://ray-tracer-backend.onrender.com', // Your actual backend URL
    // ... rest of config
};
```

#### 3.3 Commit and Push Changes
```bash
git add frontend/index.html
git commit -m "Update backend URL for production"
git push origin main
```

#### 3.4 Redeploy Frontend
1. Go to your frontend service in Render.com
2. Click **"Manual Deploy"** → **"Deploy latest commit"**
3. Wait for deployment to complete

## 🧪 Testing Your Deployment

### Test Backend
1. Visit your backend URL: `https://ray-tracer-backend.onrender.com/health`
2. Should return: `{"status": "healthy", "service": "ray-tracer-api", "version": "1.0.0"}`

### Test Frontend
1. Visit your frontend URL: `https://ray-tracer-frontend.onrender.com`
2. Open browser console (F12)
3. Look for: `✅ Backend is healthy`
4. Add some planes and click "Calculate"

### Test Full Integration
1. **Add planes** to the 3D scene
2. **Set plane types**: Receiver, Emitter, Inert
3. **Click "Calculate"**
4. **Verify contour plot** appears in modal

## 🔧 Troubleshooting

### Common Issues

#### Backend Won't Start
**Symptoms**: Build fails or service won't start
**Solutions**:
1. Check build logs in Render dashboard
2. Verify Dockerfile syntax
3. Ensure all files are in `backend/` directory
4. Check environment variables are set correctly

#### Frontend Can't Connect to Backend
**Symptoms**: Console shows "Backend is not responding"
**Solutions**:
1. Verify backend URL is correct in `CONFIG.BACKEND_URL`
2. Check backend is running and accessible
3. Ensure backend URL uses HTTPS
4. Test backend URL directly in browser

#### CORS Errors
**Symptoms**: Browser console shows CORS errors
**Solutions**:
1. Backend has CORS enabled (already configured)
2. Ensure both frontend and backend use HTTPS
3. Check that backend URL is exactly correct

#### Calculation Timeout
**Symptoms**: "Calculating..." button stays disabled
**Solutions**:
1. Reduce `num_rays` in the calculation
2. Use smaller grid sizes
3. Check backend logs for processing errors
4. Consider upgrading to paid plan for better performance

#### Build Failures
**Symptoms**: Build process fails
**Solutions**:
1. Check Dockerfile syntax
2. Verify all source files are present
3. Ensure Makefile is correct
4. Check build logs for specific errors

### Debug Steps

#### 1. Check Backend Logs
1. Go to your backend service in Render.com
2. Click **"Logs"** tab
3. Look for error messages or warnings

#### 2. Test Backend Endpoints
```bash
# Health check
curl https://your-backend.onrender.com/health

# Status check
curl https://your-backend.onrender.com/status

# Test calculation
curl -X POST https://your-backend.onrender.com/test
```

#### 3. Check Frontend Console
1. Open frontend in browser
2. Press F12 to open developer tools
3. Go to **Console** tab
4. Look for error messages or connection issues

#### 4. Verify Configuration
Check that your frontend `CONFIG` object has the correct backend URL:
```javascript
const CONFIG = {
    BACKEND_URL: 'https://your-actual-backend.onrender.com',
    // ... rest of config
};
```

## 📊 Performance Optimization

### Free Tier Limitations
- **CPU**: 0.5 vCPU
- **Memory**: 512 MB
- **Bandwidth**: 100 GB/month
- **Sleep**: Services sleep after 15 minutes of inactivity

### Optimization Tips
1. **Reduce ray count**: Use smaller `num_rays` values
2. **Smaller grids**: Use lower receiver grid sizes
3. **Efficient algorithms**: Optimize your C++ code
4. **Upgrade plan**: Consider paid plans for better performance

## 🔒 Security Considerations

### Production Deployment
1. **Use HTTPS**: Both frontend and backend should use HTTPS
2. **Environment variables**: Store sensitive data in environment variables
3. **Rate limiting**: Consider implementing rate limiting
4. **Input validation**: Backend validates all input data

### Custom Domains
1. **Add custom domain** in Render.com service settings
2. **Update DNS records** as instructed
3. **Update frontend config** with new domain
4. **SSL certificates** are automatically provisioned

## 📈 Monitoring and Maintenance

### Health Monitoring
- **Backend health**: `/health` endpoint
- **Service status**: `/status` endpoint
- **Render dashboard**: Monitor service health

### Logs and Debugging
- **Backend logs**: Available in Render dashboard
- **Frontend logs**: Browser console
- **Error tracking**: Monitor for common issues

### Updates and Maintenance
1. **Code updates**: Push to Git repository
2. **Automatic deployment**: Render auto-deploys on push
3. **Manual deployment**: Use "Manual Deploy" if needed
4. **Rollback**: Use previous deployments if issues arise

## 🆘 Support and Resources

### Render.com Documentation
- [Render.com Docs](https://render.com/docs)
- [Docker Deployment](https://render.com/docs/docker)
- [Static Sites](https://render.com/docs/static-sites)

### Common Commands
```bash
# Check service status
curl https://your-backend.onrender.com/health

# Test calculation endpoint
curl -X POST https://your-backend.onrender.com/calculate \
  -H "Content-Type: application/json" \
  -d '{"receiver_planes": {...}}'

# View logs (in Render dashboard)
# Go to your service → Logs tab
```

### Getting Help
1. **Render.com Support**: Check Render.com documentation
2. **Community**: Render.com community forums
3. **Logs**: Always check service logs first
4. **Console**: Check browser console for frontend issues

## 🎉 Success Checklist

- [ ] Backend deployed and accessible
- [ ] Frontend deployed and accessible
- [ ] Backend URL updated in frontend
- [ ] Health check returns success
- [ ] Frontend can connect to backend
- [ ] Calculation works end-to-end
- [ ] Contour plot displays correctly
- [ ] No console errors
- [ ] Performance is acceptable

Your Ray Tracer application is now fully deployed on Render.com! 🚀