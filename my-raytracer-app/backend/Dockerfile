# Dockerfile for Render.com deployment
FROM ubuntu:22.04

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive
ENV PORT=10000

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    make \
    python3 \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

# Install Python dependencies
RUN pip3 install flask flask-cors

# Set working directory
WORKDIR /app

# Copy all backend files
COPY . ./

# List files to verify copy worked
RUN echo "Files in /app:" && ls -la

# Build the C++ application directly
RUN echo "Compiling calcus.cpp..." && \
    g++ -std=c++17 -O3 calcus.cpp -o calcus -lm && \
    echo "Compilation successful!" && \
    chmod +x calcus && \
    echo "Binary is ready"

# Expose port (Render.com uses PORT environment variable)
EXPOSE $PORT

# Run the Python server
CMD ["python3", "server.py"]
