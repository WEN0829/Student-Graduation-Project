FROM nginx:alpine
WORKDIR /Student-Graduation-Project
COPY Student-Graduation-Project ./static
COPY nginx.conf /etc/nginx/conf.d/mysite.conf
RUN rm /etc/nginx/conf.d/default.conf
