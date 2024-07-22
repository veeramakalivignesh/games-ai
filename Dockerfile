# Use an official Python runtime as a parent image
FROM python:3.11-slim

# Install dependencies
RUN apt-get update && \
    apt-get install -y g++ && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Set the working directory in the container
WORKDIR /app

# Copy the current directory contents into the container at /app
COPY . /app

# Run the setup script to compile the C++ code
RUN chmod +x setup.sh
RUN ./setup.sh

# Install Python dependencies
RUN pip install --no-cache-dir -r requirements.txt

# Run the application
CMD uvicorn src.python.app:app --host 0.0.0.0 --port $PORT
