FROM debian:bookworm

# Install required tools for setting up APT repositories
RUN apt-get update && apt-get install -y \
    ca-certificates curl gnupg

# Add ZeroC nightly repository for Debian 12 (bookworm)
RUN curl -fsSL https://download.zeroc.com/GPG-KEY-zeroc-nightly | gpg --dearmor -o /etc/apt/keyrings/zeroc-nightly.gpg && \
    echo "deb [signed-by=/etc/apt/keyrings/zeroc-nightly.gpg] https://download.zeroc.com/nexus/repository/debian12-nightly nightly main" > /etc/apt/sources.list.d/zeroc-nightly.list

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
