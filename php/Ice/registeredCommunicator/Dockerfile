FROM debian:trixie

# Add ZeroC nightly repository for Debian 13 (trixie)
RUN apt-get update && apt-get install -y wget && \
    wget "https://download.zeroc.com/ice/nightly/3.9/debian13/ice-repo-3.9-nightly_1.0.0_all.deb" -O ice-repo-3.9-nightly.deb && \
    dpkg -i ice-repo-3.9-nightly.deb && \
    rm ice-repo-3.9-nightly.deb

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
