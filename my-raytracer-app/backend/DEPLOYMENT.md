# Render.com Deployment Guide

This guide explains how to deploy the Ray Tracer application to Render.com's free tier.

## Prerequisites

1. A Render.com account (free tier available)
2. Your code pushed to a Git repository (GitHub, GitLab, or Bitbucket)
3. Docker knowledge (basic)

## Deployment Steps

### 1. Prepare Your Repository

Ensure your repository structure looks like this:
```
my-raytracer-app/
├── backend/
│   ├── calcus.cpp
│   ├── Makefile
│   ├── Dockerfile
│   ├── server.py
│   └── render.yaml
└── frontend/
    └── index.html
```

### 2. Create a New Web Service on Render.com

1. Log in to [Render.com](https://render.com)
2. Click "New +" → "Web Service"
3. Connect your Git repository
4. Configure the service:

#### Basic Settings
- **Name**: `ray-tracer-api`
- **Environment**: `Docker`
- **Region**: `Oregon` (or your preferred region)
- **Branch**: `main`
- **Root Directory**: `backend`

#### Build & Deploy
- **Dockerfile Path**: `./Dockerfile`
- **Docker Context**: `./`
- **Build Command**: (leave empty)
- **Start Command**: (leave empty)

#### Environment Variables
Add these environment variables:
```
PORT=10000
RAY_TRACER_BINARY=./calcus
MAX_REQUEST_SIZE=5242880
```

#### Health Check
- **Health Check Path**: `/health`

#### Plan
- **Plan**: `Free`

### 3. Deploy

1. Click "Create Web Service"
2. Render will automatically build and deploy your application
3. Monitor the build logs for any issues
4. Once deployed, you'll get a URL like `https://ray-tracer-api.onrender.com`

## API Endpoints

Once deployed, your API will be available at:

- `GET /health` - Health check
- `GET /status` - Server status
- `POST /calculate` - Main calculation endpoint
- `POST /test` - Test with sample data

## Testing the Deployment

### Health Check
```bash
curl https://your-app.onrender.com/health
```

### Test Calculation
```bash
curl -X POST https://your-app.onrender.com/test \
  -H "Content-Type: application/json"
```

### Custom Calculation
```bash
curl -X POST https://your-app.onrender.com/calculate \
  -H "Content-Type: application/json" \
  -d '{
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
  }'
```

## Free Tier Limitations

- **CPU**: 0.5 vCPU
- **Memory**: 512 MB
- **Storage**: 1 GB
- **Bandwidth**: 100 GB/month
- **Instances**: 1 (no scaling)
- **Sleep**: Service sleeps after 15 minutes of inactivity

## Troubleshooting

### Build Issues
1. Check the build logs in Render dashboard
2. Ensure all files are in the correct directory
3. Verify Dockerfile syntax

### Runtime Issues
1. Check the service logs
2. Verify environment variables
3. Test the health endpoint

### Performance Issues
1. Reduce `num_rays` in requests for faster processing
2. Optimize your C++ code for smaller memory footprint
3. Consider upgrading to a paid plan for better performance

## Monitoring

- **Logs**: Available in Render dashboard
- **Metrics**: CPU, memory, and request metrics
- **Health**: Automatic health checks every 30 seconds

## Custom Domain

To add a custom domain:
1. Go to your service settings
2. Add your domain in the "Custom Domains" section
3. Update your DNS records as instructed
4. SSL certificate will be automatically provisioned

## Scaling

Free tier is limited to 1 instance. For scaling:
1. Upgrade to a paid plan
2. Configure auto-scaling in service settings
3. Set minimum and maximum instances

## Security

- HTTPS is enabled by default
- Environment variables are encrypted
- No persistent storage (use external database if needed)
- CORS is enabled for cross-origin requests

## Cost Optimization

- Use free tier for development and testing
- Upgrade to paid plans only when needed
- Monitor usage to avoid unexpected charges
- Use efficient algorithms to reduce processing time