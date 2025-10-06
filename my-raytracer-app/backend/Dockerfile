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

# Copy source files
COPY calcus.cpp .
COPY Makefile .
COPY server.py .

# Build the C++ application
RUN make clean && make

# Make the binary executable
RUN chmod +x calcus

# Expose port (Render.com uses PORT environment variable)
EXPOSE $PORT

# Run the Python server
CMD ["python3", "server.py"]