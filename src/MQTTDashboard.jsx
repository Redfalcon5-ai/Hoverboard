import React, { useState, useEffect } from "react";
import mqtt from "mqtt";
import "./MQTTDashboard.css";

const MQTTDashboard = () => {
  const [pwm1, setPwm1] = useState(0);
  const [pwm2, setPwm2] = useState(0);
  const [client, setClient] = useState(null);
  const [isConnected, setIsConnected] = useState(false);
  const [authenticated, setAuthenticated] = useState(false);
  const [inputPassword, setInputPassword] = useState("");
  const [attempted, setAttempted] = useState(false);


  const correctPassword = "nigger"; 

  useEffect(() => {
    if (!authenticated) return;

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
  }, [authenticated]);

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

  if (!authenticated) {
    return (
      <div className="dashboard-background">
        <div className="dashboard-card">
          <h2 className="dashboard-title">🔒 Access Locked</h2>
          {/* <p className="auth-subtext">Enter password to continue</p> */}
  
          <div className="auth-input-group">
            <input
              type="password"
              value={inputPassword}
              onChange={(e) => {
                setInputPassword(e.target.value);
                setAttempted(false); // Clear error while typing
              }}
              placeholder="Enter password"
              className="password-input"
            />
            {attempted && inputPassword !== correctPassword && (
              <p className="auth-error">Incorrect password. Please try again.</p>
            )}
          </div>
  
          <button
            className="unlock-button"
            onClick={() => {
              setAttempted(true);
              if (inputPassword === correctPassword) {
                setAuthenticated(true);
              }
            }}
          >
            Unlock
          </button>
        </div>
      </div>
    );
  }
  
  
  

  return (
    <div className="dashboard-background">
      <div className="dashboard-card">
        <h1 className="dashboard-title">bot</h1>

        <div className="control-group">
          <label className="label">
            PWM Channel 1: <span className="label-value">{pwm1}</span>
          </label>
          <input
            className="slider"
            type="range"
            min="-30"
            max="30"
            value={pwm1}
            onChange={(e) => setPwm1(Number(e.target.value))}
          />
        </div>

        <div className="control-group">
          <label className="label">
            PWM Channel 2: <span className="label-value">{pwm2}</span>
          </label>
          <input
            className="slider"
            type="range"
            min="-30"
            max="30"
            value={pwm2}
            onChange={(e) => setPwm2(Number(e.target.value))}
          />
        </div>

        <div className="button-group">
          <button
            onClick={sendPWM}
            disabled={!isConnected}
            className="button"
            style={{ opacity: isConnected ? 1 : 0.5 }}
          >
            Send PWM
          </button>

          <button
            onClick={stopAll}
            disabled={!isConnected}
            className="stop-button"
            style={{ opacity: isConnected ? 1 : 0.5 }}
          >
            🛑 STOP
          </button>
        </div>

        <p className="status">
          Status:{" "}
          <span style={{ color: isConnected ? "#00FFAA" : "#FFA500" }}>
            {isConnected ? "Connected" : "Connecting..."}
          </span>
        </p>
      </div>
    </div>
  );
};

export default MQTTDashboard;
