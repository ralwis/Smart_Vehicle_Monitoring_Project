// Create Speed Gauge
function createSpeedGauge() {
    var gauge = new RadialGauge({
        renderTo: 'gauge-speed',
        width: 300,
        height: 300,
        units: "Kmph",
        minValue: 0,
        maxValue: 60,
        valueInt: 2,
        majorTicks: [
        "0",
        "5",
        "10",
        "15",
        "20",
        "25",
        "30",
        "35",
        "40",
        "45",
        "50",
        "55",
        "60"
    ],
    minorTicks: 5,
    strokeTicks: true,
    highlights: [
        {
            "from": 45,
            "to": 60,
            "color": "rgba(200, 50, 50, .75)"
        }
    ],
    colorPlate: "#fff",
    borderShadowWidth: 0,
    borders: false,
    needleType: "arrow",
    needleWidth: 2,
    needleCircleSize: 7,
    needleCircleOuter: true,
    needleCircleInner: false,
    animationDuration: 1500,
    animationRule: "linear"
});
    return gauge;
}

// Create Distance Gauge
function createDistanceGauge(){
    var $text = $('p.number'),
  $input  = $('input[type=number]'),
  endVal  = 0,
  currVal = 0,
  obj = {};

obj.getTextVal = function() {
  return parseInt(currVal, 10);
};

obj.setTextVal = function(val) {
  currVal = parseInt(val, 10);
  $text.text(currVal);
};

obj.setTextVal(0);

var animate = function(e) {
  e.preventDefault();
  currVal = 0; // Reset this every time
  endVal = $input.val();

  TweenLite.to(obj, 7, {setTextVal: endVal, ease: Power1.easeOut});
};

$('#number-input').on('submit', animate);
}


