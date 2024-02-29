#!/bin/bash

cd "/Users/joshbrake/dev/E155/"
quarto render web/

rsync --update -rv web/_site/ "/Users/joshbrake/Library/CloudStorage/GoogleDrive-jbrake@g.hmc.edu/Shared drives/wwwbrake/class/e155/fa23"

sleep 10