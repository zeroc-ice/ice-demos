FROM debian:bookworm

# Install required tools for setting up APT repositories
RUN apt-get update && apt-get install -y \
    ca-certificates curl gnupg

# Add ZeroC 3.8 repository for Debian 12 (bookworm)
RUN curl -fsSL https://download.zeroc.com/ice/3.8/RPM-GPG-KEY-zeroc-release | gpg --dearmor -o /etc/apt/keyrings/zeroc-3.8.gpg && \
    echo "deb [signed-by=/etc/apt/keyrings/zeroc-3.8.gpg] https://download.zeroc.com/ice/3.8/debian12 stable main" > /etc/apt/sources.list.d/zeroc-3.8.list

# Install Ice for PHP, PHP-FPM, and Nginx
RUN apt-get update && apt-get install -y \
    php-zeroc-ice php-fpm nginx-full \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

# Disable the default server
RUN rm /etc/nginx/sites-enabled/default
# Use a custom Nginx configuration
COPY nginx.conf /etc/nginx/conf.d/default.conf

ENTRYPOINT ["/entrypoint.sh"]
