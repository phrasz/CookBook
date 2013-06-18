#!/bin/bash
awk '{ print }' /etc/passwd
awk '{ print $0 }' /etc/passwd
