let video;
let poseNet;
let poses = [];
let skeletons = [];
let numPose;

var capture;
var options;

let sawPoses = false;

var shiftrkey = '9d5a1aac';
var shiftrsecret = 'ebf8fab1ad528bb2';


function setup() {

    /*
    if (!navigator.mediaDevices || !navigator.mediaDevices.enumerateDevices) {
        console.log("enumerateDevices() not supported.");
        return;
    }

// List cameras and microphones.

    navigator.mediaDevices.enumerateDevices()
        .then(function(devices) {
            devices.forEach(function(device) {
                console.log(device.kind + ": " + device.label +
                    " id = " + device.deviceId);
            });
        })
        .catch(function(err) {
            console.log(err.name + ": " + err.message);
        });

         // var options = {
  //   video: {
  //     optional: [{
  //       sourceId: 'put_desired_source_id_here'
  //     }]
  //   }
  // };




*/









    frameRate(5);
    createCanvas(640, 480);

    var options = {
    video: {
       optional: [{
         sourceId: 'put_desired_source_id_here'
        }]
      }
    };

   video = createCapture(options);
   video.size(width, height);


    // Create a new poseNet method with a single detection
    poseNet = ml5.poseNet(video, modelReady);
    // This sets up an event that fills the global variable "poses"
    // with an array every time new poses are detected
    poseNet.on('pose', function (results) {
        poses = results;
    });
    // Hide the video element, and just show the canvas
    video.hide();

    client = mqtt.connect('mqtt://' + shiftrkey + ':' + shiftrsecret + '@broker.shiftr.io', {
        clientId: 'p5js'
    });

    client.on('connect', function() {
        console.log('client has connected!');

        client.subscribe('/hello/output/lys');
    });

    client.on('message', function(topic, message) {
        messageReceived(topic, message);
    });
}


function modelReady() {
    select('#status').html('Model Loaded');
}

function draw() {
    image(video, 0, 0, width, height);

    // We can call both functions to draw all keypoints and the skeletons
    drawKeypoints();
    drawSkeleton();

    numPose = poses.length;

    if (numPose > 0) {
        if(sawPoses){
            return;
        }
        else{
            sawPoses = true;
        }
        client.publish('/hello/output/lys', '1');
        console.log("1")
    }
    if (numPose < 1) {
        if(sawPoses){
            sawPoses = false;
        }
        else{
            return;
        }
        client.publish('/hello/output/lys', '0');
        console.log("0")
    }

}

// A function to draw ellipses over the detected keypoints
function drawKeypoints()  {
    // Loop through all the poses detected
    for (let i = 0; i < poses.length; i++) {
        // For each pose detected, loop through all the keypoints
        for (let j = 0; j < poses[i].pose.keypoints.length; j++) {
            // A keypoint is an object describing a body part (like rightArm or leftShoulder)
            let keypoint = poses[i].pose.keypoints[j];
            // Only draw an ellipse is the pose probability is bigger than 0.2
            if (keypoint.score > 0.2) {
                fill(255, 0, 0);
                noStroke();
                ellipse(keypoint.position.x, keypoint.position.y, 10, 10);
            }
        }
    }
}

// A function to draw the skeletons
function drawSkeleton() {
    // Loop through all the skeletons detected
    for (let i = 0; i < poses.length; i++) {
        // For every skeleton, loop through all body connections
        for (let j = 0; j < poses[i].skeleton.length; j++) {
            let partA = poses[i].skeleton[j][0];
            let partB = poses[i].skeleton[j][1];
            stroke(255, 0, 0);
            line(partA.position.x, partA.position.y, partB.position.x, partB.position.y);
        }

    }
}

function publishMessage(topic, message) {
    client.publish(String(topic), String(message));
}

// DEN HER FUNKTION SKYDER HVER GANG VI MODTAGER EN BESKED
function messageReceived(t, m) {
    var topic = t.toString();
    var message = m.toString();

}



