from flask import Flask
from flask_sqlalchemy import SQLAlchemy
from flask_marshmallow import Marshmallow
from flask_restful import Api
from flask_httpauth import HTTPBasicAuth
import os
# Init app
app = Flask(__name__)

#basedir = os.path.abspath(os.path.dirname(__file__))
# Database
app.config['SECRET_KEY'] = 'the quick brown fox jumps over the lazy dog'
app.config['SQLALCHEMY_DATABASE_URI'] = "sqlite:///db.sqlite"
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
# Init db
db = SQLAlchemy(app)
# Init ma
ma = Marshmallow(app)


# security
auth = HTTPBasicAuth()

# users
users = {"admin": "password", "ricveal": "1234" , "Admin" :"Abc113377"}