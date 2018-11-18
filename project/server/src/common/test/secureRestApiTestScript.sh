#!/usr/bin/env bash
PORT=7778
IP=127.0.0.1
METHOD=https

run_curl () {
        MSG=$1
        VERB=$2
        API_PATH=$3

        printf "%-30s%-4s: " "$MSG"
        curl -k -X $VERB $METHOD://$IP:$PORT$API_PATH
        echo
}

run_curl "file" "GET" "/superviseur/file"
run_curl "del chanson" "DELETE" "/superviseur/chanson/3"
run_curl "inversion" "POST" "/superviseur/inversion"
run_curl "volume" "GET" "/superviseur/volume"
run_curl "augmenter" "POST" "/superviseur/volume/augmenter/1"
run_curl "diminuer" "POST" "/superviseur/volume/diminuer/1"
run_curl "sourdine act" "POST" "/superviseur/volume/sourdine/activer"
run_curl "sourdine des" "POST" "/superviseur/volume/sourdine/desactiver"
run_curl "stats " "GET" "/superviseur/statistiques"
run_curl "bloque " "POST" "/superviseur/bloquer"
run_curl "debloque " "POST" "/superviseur/debloquer"
run_curl "listenoire" "GET" "/superviseur/listenoire"
run_curl "login" "POST" "/superviseur/login"
run_curl "logout" "POST" "/superviseur/logout"
run_curl "change mdp" "POST" "/superviseur/changement_mdp"
