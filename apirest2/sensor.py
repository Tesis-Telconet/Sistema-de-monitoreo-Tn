from app import db, ma
from datetime import datetime

# Clase Temperatura
class Temperatura(db.Model):
  id = db.Column(db.Integer, primary_key=True)
  temperature = db.Column(db.Float)
  login= db.Column(db.String)
  fecha = db.Column(db.DateTime)
  date = db.Column(db.DateTime)
  
  def __init__(self, temperature, login, fecha=None, date=None):
    self.temperature = temperature
    self.login = login
    if fecha is None:
           fecha = datetime.now()
    self.fecha = fecha
    if date is None:
           date = datetime.utcnow()
    self.date = date

# Product Schema
class TemperaturaSchema(ma.ModelSchema):
  class Meta:
    model = Temperatura

# Clase Humedad
class Humedad(db.Model):
  id = db.Column(db.Integer, primary_key=True)
  humidity = db.Column(db.Float)
  login= db.Column(db.String)
  fecha = db.Column(db.DateTime)
  date = db.Column(db.DateTime)
  
  def __init__(self, humidity, login, fecha=None, date=None):
    self.humidity = humidity
    self.login = login
    if fecha is None:
           fecha = datetime.now()
    self.fecha = fecha
    if date is None:
           date = datetime.utcnow()
    self.date = date

# Product Schema
class HumedadSchema(ma.ModelSchema):
  class Meta:
    model = Humedad


# Clase gas
class Gas(db.Model):
  id = db.Column(db.Integer, primary_key=True)
  cantidad_gas = db.Column(db.Float)
  login= db.Column(db.String)
  fecha = db.Column(db.DateTime)
  date = db.Column(db.DateTime)
  
  def __init__(self, cantidad_gas, login, fecha=None, date=None):
    self.cantidad_gas = cantidad_gas
    self.login = login
    if fecha is None:
           fecha = datetime.now()
    self.fecha = fecha
    if date is None:
           date = datetime.utcnow()
    self.date = date

# Product Schema
class GasSchema(ma.ModelSchema):
  class Meta:
    model = Gas


# Clase Corriente
class Corriente(db.Model):
  id = db.Column(db.Integer, primary_key=True)
  corriente = db.Column(db.Float)
  login= db.Column(db.String)
  fecha = db.Column(db.DateTime)
  date = db.Column(db.DateTime)
  
  def __init__(self, corriente, login, fecha=None, date=None):
    self.corriente = corriente
    self.login = login
    if fecha is None:
           fecha = datetime.now()
    self.fecha = fecha
    if date is None:
           date = datetime.utcnow()
    self.date = date

# Product Schema
class CorrienteSchema(ma.ModelSchema):
  class Meta:
    model = Corriente


# Clase Voltaje
class Voltaje(db.Model):
  id = db.Column(db.Integer, primary_key=True)
  voltaje = db.Column(db.Float)
  login= db.Column(db.String)
  fecha = db.Column(db.DateTime)
  date = db.Column(db.DateTime)
  
  def __init__(self, voltaje, login, fecha=None, date=None):
    self.voltaje = voltaje
    self.login = login
    if fecha is None:
           fecha = datetime.now()
    self.fecha = fecha
    if date is None:
           date = datetime.utcnow()
    self.date = date

# Product Schema
class VoltajeSchema(ma.ModelSchema):
  class Meta:
    model = Voltaje


# Clase Log
class Log(db.Model):
  id = db.Column(db.Integer, primary_key=True)
  log= db.Column(db.String)
  fecha = db.Column(db.DateTime)
  
  def __init__(self, log, fecha=None):
    self.log = log
    if fecha is None:
           fecha = datetime.now()
    self.fecha = fecha

# Product Schema
class LogSchema(ma.ModelSchema):
  class Meta:
    model = Log



