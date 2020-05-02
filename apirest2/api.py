from flask_restful import Resource
from flask import Flask, request, jsonify, render_template, session, escape, request, redirect, url_for, g
from app import app, db, auth, users
from sensor import Temperatura, TemperaturaSchema, Humedad, HumedadSchema, Gas, GasSchema, Corriente, CorrienteSchema, Voltaje, VoltajeSchema, Log, LogSchema
import os
import sqlalchemy

@app.before_request
def before_request():
  g.user:None
  if 'user' in session:
    g.user = session['user']
    
@app.route('/', methods = ['GET', 'POST'])

def logeo():
  if request.method == 'POST':
    session.pop('user', None)

    if request.form['password']== 'Abc113377':
      session['user'] = request.form['username']
      return redirect(url_for("index"))
 
  return render_template('login.html')


@app.route('/index')
def index():
  if g.user:
    return render_template('index.html', user=session['user'])
  else:
   return redirect(url_for('logeo'))
   




@app.route('/salir')
def salir():
  session.pop('user', None)
  return redirect(url_for('logeo'))

@auth.get_password
def get_pw(username):
    if username in users:
        return users.get(username) 
    return None

#### TABLAS DE DATOS ####

@app.route('/temp')
def temp():
  if g.user:
    return render_template('temp.html', user=session['user'])
  return redirect(url_for('logeo'))
  
  

@app.route('/hum')   
def hum():
  if g.user:
    return render_template('hum.html', user=session['user'])
  return redirect(url_for('logeo'))

@app.route('/cor')   
def cor():
  if g.user:
    return render_template('cor.html', user=session['user'])
  return redirect(url_for('logeo'))

@app.route('/vol')   
def vol():
  if g.user:
    return render_template('vol.html', user=session['user'])
  return redirect(url_for('logeo'))
    
@app.route('/gas')    
def gas():
  if g.user:
    return render_template('gas.html', user=session['user'])
  return redirect(url_for('logeo'))

@app.route('/log')   
def log():
  if g.user:
    return render_template('log.html', user=session['user'])
  return redirect(url_for('login.html'))

@app.route('/ame')
def ame():
  if g.user:
    return render_template('ame.html', user=session['user'])
  return redirect(url_for('logeo'))


#### GRAFICOS ####

@app.route('/chart')    
def chart():
  if g.user:
    return render_template('chart.html', user=session['user'])
  return redirect(url_for('logeo'))

@app.route('/charh')   
def charh():
  if g.user:
    return render_template('charh.html', user=session['user'])
  return redirect(url_for('logeo'))

@app.route('/charg')   
def charg():
  if g.user:
    return render_template('charg.html', user=session['user'])
  return redirect(url_for('logeo'))

@app.route('/charc')    
def charc():
  if g.user:
    return render_template('charc.html', user=session['user'])
  return redirect(url_for('logeo'))

@app.route('/charcg')    
def charcg():
  if g.user:
    return render_template('charcg.html', user=session['user'])
  return redirect(url_for('logeo'))

@app.route('/charv')    
def charv():
  if g.user:
    return render_template('charv.html', user=session['user'])
  return redirect(url_for('logeo'))

@app.route('/charvg')    
def charvc():
  if g.user:
    return render_template('charvg.html', user=session['user'])
  return redirect(url_for('logeo'))

temperatura_schema = TemperaturaSchema()
temperaturas_schema = TemperaturaSchema(many=True)

# Ingresar Temperaturas
@app.route('/SensorData/temperatura', methods=['POST'])
@auth.login_required
def add_temperatura():
  args = request.get_json(force=True)
  sensor_read = Temperatura(args["temperature"], args["login"])
  db.session.add(sensor_read)
  db.session.commit()
  return 'Temperature Registrada', 200

# Obtener todas las temperaturas (tablas)
@app.route('/SensorData/temperatura', methods=['GET'])
#@auth.login_required
def get_temperaturas():
  all_temperaturas = Temperatura.query.all()
  return temperaturas_schema.jsonify(all_temperaturas)

# Obtener temperatura individual (graficos)
@app.route('/SensorData/temperatura/<string:login>', methods=['GET'])
#@auth.login_required
def get_temperatura(login):
  all_temperaturas = Temperatura.query.filter_by(login=login )
  seq = [[item.date, item.temperature] for item in all_temperaturas]
  return jsonify(seq)


# Init schema
humedad_schema = HumedadSchema()
humedades_schema = HumedadSchema(many=True)


# Ingresar humedades
@app.route('/SensorData/humedad', methods=['POST'])
@auth.login_required
def add_humedad():
  args = request.get_json(force=True)
  sensor_read = Humedad(args["humidity"], args["login"])
  db.session.add(sensor_read)
  db.session.commit()
  return 'Humedad Registrada', 200

#Obtener todas las Humedades (tablas)
@app.route('/SensorData/humedad', methods=['GET'])
@auth.login_required
def get_humedades():
  all_humedades = Humedad.query.all()
  return humedades_schema.jsonify(all_humedades)

# Obtener humedad individual (graficos)
@app.route('/SensorData/humedad/<string:login>', methods=['GET'])
@auth.login_required
def get_humedad(login):
  all_humedades = Humedad.query.filter_by(login=login )
  seq = [[item.date, item.humidity] for item in all_humedades]
  return jsonify(seq)


# Init schema
gas_schema = GasSchema()
gases_schema = GasSchema(many=True)


# Ingresar Gas
@app.route('/SensorData/gas', methods=['POST'])
@auth.login_required
def add_gas():
  args = request.get_json(force=True)
  sensor_read = Gas(args["cant_gas"], args["login"])
  db.session.add(sensor_read)
  db.session.commit()
  return 'Cantidad Gas Registrado', 200

#Obtener todas los valores de gases (tablas)
@app.route('/SensorData/gas', methods=['GET'])
@auth.login_required
def get_gases():
  all_gases = Gas.query.all()
  return gases_schema.jsonify(all_gases)

# Obtener valor individual de gas (graficos)
@app.route('/SensorData/gas/<string:login>', methods=['GET'])
@auth.login_required
def get_gas(login):
  all_temperaturas = Gas.query.filter_by(login=login )
  seq = [[item.date, item.cantidad_gas] for item in all_temperaturas]
  return jsonify(seq)


# Init schema
corriente_schema = CorrienteSchema()
corrientes_schema =CorrienteSchema(many=True)


# Ingresar Corrientes
@app.route('/SensorData/corriente', methods=['POST'])
@auth.login_required
def add_corriente():
  args = request.get_json(force=True)
  sensor_read = Corriente(args["corriente"], args["login"])
  db.session.add(sensor_read)
  db.session.commit()
  return 'Corriente Registrada', 200

#Obtener todas las corrientes (tablas)
@app.route('/SensorData/corriente', methods=['GET'])
@auth.login_required
def get_corrientes():
  all_corrientes = Corriente.query.all()
  return corrientes_schema.jsonify(all_corrientes)

# Obtener corriente individual (graficos)
@app.route('/SensorData/corriente/<string:login>', methods=['GET'])
@auth.login_required
def get_corriente(login):
  all_corrientes = Corriente.query.filter_by(login=login )
  seq = [[item.date, item.corriente] for item in all_corrientes]
  return jsonify(seq)


# Init schema
voltaje_schema = VoltajeSchema()
voltajes_schema = VoltajeSchema(many=True)


# Ingresar Voltajes
@app.route('/SensorData/voltaje', methods=['POST'])
@auth.login_required
def add_voltaje():
  args = request.get_json(force=True)
  sensor_read = Voltaje(args["voltaje"], args["login"])
  db.session.add(sensor_read)
  db.session.commit()
  return 'Voltaje Registrado', 200

#Obtener todos las voltajes (tablas)
@app.route('/SensorData/voltaje', methods=['GET'])
@auth.login_required
def get_voltajes():
  all_voltajes = Voltaje.query.all()
  return voltajes_schema.jsonify(all_voltajes)

# Obtener voltaje individual (graficos)
@app.route('/SensorData/voltaje/<string:login>', methods=['GET'])
@auth.login_required
def get_voltaje(login):
  all_voltajes = Voltaje.query.filter_by(login=login )
  seq = [[item.date, item.voltaje] for item in all_voltajes]
  return jsonify(seq)

# Init schema
log_schema = LogSchema()
logs_schema =LogSchema(many=True)


# Ingresar Mensajes
@app.route('/SensorData/log', methods=['POST'])
@auth.login_required
def add_mensajes():
  args = request.get_json(force=True)
  sensor_read = Log(args["log"])
  db.session.add(sensor_read)
  db.session.commit()
  return 'Evento Registrado', 200

#Obtener todas los Mensajes (tablas)
@app.route('/SensorData/log', methods=['GET'])
#@auth.login_required
def get_mensajes():
  all_mensajes = Log.query.all()
  return logs_schema.jsonify(all_mensajes)



# Run Server
if __name__ == '__main__':
  app.run(host="0.0.0.0",debug=True)