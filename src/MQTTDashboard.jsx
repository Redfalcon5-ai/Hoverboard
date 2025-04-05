import React, { useState, useEffect } from "react";
import mqtt from "mqtt";

const MQTTDashboard = () => {
  const [pwm1, setPwm1] = useState(0);
  const [pwm2, setPwm2] = useState(0);
  const [client, setClient] = useState(null);
  const [isConnected, setIsConnected] = useState(false);

  useEffect(() => {
    const options = {
      clean: true,
      connectTimeout: 4000,
      clientId: "web_dashboard_" + Math.random().toString(16).substr(2, 8),
      username: "hivemq.webclient.1743373482038",
      password: "3a2;j1GK,ploYEL8i#&J",
      protocol: "wss",
      
    };


    const mqttClient = mqtt.connect(
      "wss://9f1c22f41e404f04b884c3123ae4200e.s1.eu.hivemq.cloud:8884/mqtt",
      options
    );

    mqttClient.on("connect", () => {
      console.log("Connected to MQTT broker");
      setIsConnected(true);
    });

    mqttClient.on("error", (err) => {
      console.error("Connection error: ", err);
      mqttClient.end();
    });

    setClient(mqttClient);

    return () => mqttClient.end();
  }, []);

  const sendPWM = () => {
    if (client && isConnected) {
      const payload = `${pwm1},${pwm2}`;
      client.publish("DRIVE_CMD", payload, { qos: 1 });
      console.log("Sent:", payload);
    }
  };

  const stopAll = () => {
    setPwm1(0);
    setPwm2(0);
    if (client && isConnected) {
      const payload = "0,0";
      client.publish("DRIVE_CMD", payload, { qos: 1 });
      console.log("Emergency STOP sent!");
    }
  };

  return (
    <div style={styles.background}>
      <div style={styles.card}>
        <h1 style={styles.title}> bot</h1>

        <div style={styles.controlGroup}>
          <label style={styles.label}>PWM Channel 1: <span style={styles.value}>{pwm1}</span></label>
          <input
            style={styles.slider}
            type="range"
            min="-30"
            max="30"
            value={pwm1}
            onChange={(e) => setPwm1(Number(e.target.value))}
          />
        </div>

        <div style={styles.controlGroup}>
          <label style={styles.label}>PWM Channel 2: <span style={styles.value}>{pwm2}</span></label>
          <input
            style={styles.slider}
            type="range"
            min="-30"
            max="30"
            value={pwm2}
            onChange={(e) => setPwm2(Number(e.target.value))}
          />
        </div>

        <div style={styles.buttonGroup}>
          <button
            onClick={sendPWM}
            disabled={!isConnected}
            style={{ ...styles.button, opacity: isConnected ? 1 : 0.5 }}
          >
            Send PWM
          </button>

          <button
            onClick={stopAll}
            disabled={!isConnected}
            style={{ ...styles.stopButton, opacity: isConnected ? 1 : 0.5 }}
          >
            🛑 STOP
          </button>
        </div>

        <p style={styles.status}>
          Status:{" "}
          <span style={{ color: isConnected ? "#00FFAA" : "#FFA500" }}>
            {isConnected ? "Connected" : "Connecting..."}
          </span>
        </p>
      </div>
    </div>
  );
};

const styles = {
  background: {
    height: "100vh",
    background: "radial-gradient(circle at 20% 30%, #0b1a2b, #060d13)",
    color: "#FFFFFF",
    display: "flex",
    alignItems: "center",
    justifyContent: "center",
    fontFamily: "'Orbitron', sans-serif",
    padding: "1rem",
  },
  card: {
    background: "rgba(255, 255, 255, 0.05)",
    border: "1px solid rgba(255, 255, 255, 0.1)",
    borderRadius: "20px",
    padding: "2rem",
    width: "100%",
    maxWidth: "500px",
    backdropFilter: "blur(12px)",
    boxShadow: "0 0 20px rgba(0, 255, 255, 0.2)",
  },
  title: {
    textAlign: "center",
    fontSize: "1.8rem",
    marginBottom: "1.5rem",
    color: "#00BFFF",
  },
  controlGroup: {
    marginBottom: "1.5rem",
  },
  label: {
    display: "block",
    fontWeight: "bold",
    marginBottom: "0.5rem",
  },
  value: {
    fontWeight: "normal",
    color: "#00FFAA",
  },
  slider: {
    width: "100%",
    accentColor: "#00FFAA",
  },
  buttonGroup: {
    display: "flex",
    justifyContent: "space-between",
    gap: "1rem",
    marginTop: "1rem",
  },
  button: {
    flex: 1,
    backgroundColor: "#0066ff",
    color: "white",
    border: "none",
    padding: "0.75rem 1rem",
    fontSize: "1rem",
    borderRadius: "10px",
    cursor: "pointer",
    fontWeight: "bold",
    transition: "background-color 0.3s ease",
  },
  stopButton: {
    flex: 1,
    backgroundColor: "#ff3b3b",
    color: "white",
    border: "none",
    padding: "0.75rem 1rem",
    fontSize: "1rem",
    borderRadius: "10px",
    cursor: "pointer",
    fontWeight: "bold",
    transition: "background-color 0.3s ease",
  },
  status: {
    marginTop: "1.5rem",
    textAlign: "center",
    fontSize: "1rem",
  },
};

export default MQTTDashboard;
