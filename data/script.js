fetch("/api/data")
    .then(response => response.json())
    .then(data =>
{
    document.getElementById("networks").textContent = data.networks;
    document.getElementById("bestChannel").textContent = data.bestChannel;
    document.getElementById("congestionScore").textContent = data.congestionScore;
    document.getElementById("relativeCongestion").textContent = data.relativeCongestion;

    const table = document.getElementById("networkTable");
    data.networkList.forEach(network => { 
        const row = document.createElement("tr");
        row.innerHTML = `
            <td> ${network.ssid} </td>
            <td> ${network.signal} </td>
            <td> ${network.rssi} </td>
            <td> ${network.channel} </td>
           <td> ${network.security} </td>
       `;

       table.appendChild(row);
    });
});
