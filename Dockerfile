FROM python:3.9-alpine

WORKDIR /app

ADD . /app

RUN pip install --trusted-host pypi.python.org -r requirements.txt

EXPOSE 8888

CMD ["python", "python_flask.py"]
