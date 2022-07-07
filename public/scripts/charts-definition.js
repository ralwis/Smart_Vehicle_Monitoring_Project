// Create the charts when the web page loads
window.addEventListener('load', onload);

function onload(event){
  chartS = createSpeedChart();
  chartD = createDistanceChart();
}

// Create Speed Chart
function createSpeedChart(){
  
  var chart = new Highcharts.Chart({
    chart:{ 
      renderTo:'chart-speed',
      type: 'spline' 
    },
    series: [
      {
        name: 'Sensor Readings'
      }
    ],
    title: { 
      text: undefined
    },
    plotOptions: {
      line: { 
        animation: false,
        dataLabels: { 
          enabled: true 
        }
      }  
    },
    time: {
      useUTC: false,
      timezone: 'Asia/Calcutta',
    },
    xAxis: {
      type: 'datetime',
      dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
      title: { 
        text: 'Speed' 
      }
    },
    credits: { 
      enabled: false 
    }
  });
  return chart;
}

// Create Distance Chart
function createDistanceChart(){
  var chart = new Highcharts.Chart({
    chart:{ 
      renderTo:'chart-distance',
      type: 'spline'  
    },
    series: [{
      name: 'Sesor Readings'
    }],
    title: { 
      text: undefined
    },    
    plotOptions: {
      line: { 
        animation: false,
        dataLabels: { 
          enabled: true 
        }
      },
      series: { 
        color: '#50b8b4' 
      }
    },
    time: {
      useUTC: false,
      timezone: 'Asia/Calcutta',
    },
    xAxis: {
      type: 'datetime',
      dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
      title: { 
        text: 'Distance (m)' 
      }
    },
    credits: { 
      enabled: false 
    }
  });
  return chart;
}

