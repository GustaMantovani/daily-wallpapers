#!/bin/bash
export DISPLAY=:0
export XAUTHORITY=/run/user/1000/.mutter-Xwaylandauth.FJ82G2
export DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/1000/bus
cat ~/.dw/dw.conf | ~/.dw/a.out
