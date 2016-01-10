#!/bin/sh

cd LKM
make clean
rm -f Command.h
rm -f Structure.h
rm -f KmaAPI.h

cd ../APP
make clean
rm -f Command.h
rm -f Structure.h
rm -f KmaAPI.h
