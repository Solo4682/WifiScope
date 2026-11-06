fetch("/api/data")
    .then(response => response.json())
    .then(data =>
{
    document.getElementById("networks").textContent = data.networks;
    document.getElementById("bestChannel").textContent = data.bestChannel;
    document.getElementById("congestionScore").textContent = data.congestionScore;
    document.getElementById("relativeCongestion").textContent = data.relativeCongestion;
});