#!/bin/bash
set -e

# Start PHP-FPM in the background
php-fpm8.2 -D

# Start Nginx in the foreground
exec nginx -g 'daemon off;'
