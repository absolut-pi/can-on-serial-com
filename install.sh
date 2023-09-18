#!/bin/bash
SERVICE_NAME="can-on-serial-com.service"

sudo systemctl disable "$SERVICE_NAME"
sudo systemctl stop "$SERVICE_NAME"

sudo cp -f "./$SERVICE_NAME" "/lib/systemd/system/$SERVICE_NAME"

sudo systemctl daemon-reload
sudo systemctl enable "$SERVICE_NAME"
sudo systemctl start "$SERVICE_NAME"
