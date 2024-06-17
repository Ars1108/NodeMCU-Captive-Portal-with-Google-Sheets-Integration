# NodeMCU-Captive-Portal-with-Google-Sheets-Integration
This project creates a captive portal using NodeMCU where users enter details like name, address, and contact number. These details are stored directly in a Google Sheet with date and time stamps. This setup is ideal for collecting user information in public WiFi scenarios such as cafes, hotels, or events.

Features
Captive Portal: When users connect to the WiFi network, they are redirected to a web page to enter their details.
Data Collection: Collects user details including name, address, and contact number.
Google Sheets Integration: Automatically stores the collected data in a Google Sheet for easy access and management.
Timestamping: Each entry is logged with the current date and time for accurate record-keeping.

Components Used
NodeMCU: A low-cost open-source IoT platform that is programmable using the Arduino IDE.
Google Sheets: For storing and managing the user data collected via the captive portal.
Google Apps Script: Used to create a web app that receives the data from the NodeMCU and logs it into the Google Sheet.

Benefits
Easy Deployment: Simple and cost-effective way to collect user information.
Real-time Data Logging: Automatically logs data with timestamps for better tracking.
Customizable: Easily modify the form fields and data handling as per your requirements.
