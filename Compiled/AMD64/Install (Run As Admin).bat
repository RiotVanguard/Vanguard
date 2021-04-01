@echo off
sc create Vanguard binPath= "%~dp0\Vanguard.sys" type= kernel
sc start Vanguard