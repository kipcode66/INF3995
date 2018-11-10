#!/usr/bin/env bash
if [ -z "$IP" ]
then
        IP=4433
fi
curl -X POST -k --header "Content-Type: application_gjson" https://127.0.0.1:$IP/superviseur/changement_mdp --data "{\"ancien\": \"pass1\", \"nouveau\": \"passasdf\"}"

