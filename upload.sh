#!/bin/bash

# Replace these values with your specifics
local_folder="./"
remote_user="your_remote_user"
remote_host="your_remote_host"
remote_folder="your_remote_folder"

# Run rsync to synchronize the folders
rsync -avz -e ssh "." "583b:/home/leeyongs/proj/"
