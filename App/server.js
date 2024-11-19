const express = require("express");
const http = require("http");
const { Server } = require("socket.io");
const { SerialPort } = require("serialport");
const { ReadlineParser } = require("@serialport/parser-readline");

// Configure Serial port
const port = new SerialPort({ path: "COM7", baudRate: 9600 }); // Replace COM7 with your Arduino port
const parser = port.pipe(new ReadlineParser({ delimiter: "\n" }));
 
// Configure Express app
const app = express();
const server = http.createServer(app);
const io = new Server(server);

// Serve the web app
app.use(express.static("public"));

// Buffer to store concatenated data
let buffer = "";
let timer = null;

// Function to send concatenated data
function sendData() {
  if (buffer.length > 0) {
    const timestamp = new Date().toISOString();
    io.emit("data", { timestamp, data: buffer }); // Emit concatenated data to the web app
    buffer = ""; // Clear the buffer
  }
}

// Handle incoming data from Serial port
parser.on("data", (data) => {
  buffer += data.trim(); // Add the character to the buffer

  // Start or reset the timer
  if (!timer) {
    timer = setTimeout(() => {
      sendData(); // Send the concatenated data after 2 seconds
      timer = null; // Reset the timer
    }, 2000);
  }
});

// Handle client connection
io.on("connection", (socket) => {
  console.log("A user connected");

  socket.on("disconnect", () => {
    console.log("A user disconnected");
  });
});

// Start the server
server.listen(3000, () => {
  console.log("Server is running on http://localhost:3000");
});
