function ledOn() {
  fetch('/led/on').then(res => console.log('LED ON'));
}

function ledOff() {
  fetch('/led/off').then(res => console.log('LED OFF'));
}
