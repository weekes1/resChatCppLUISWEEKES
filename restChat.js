// Rest based chat client
// Jim Skon 2022
// Kenyon College

var baseUrl = 'http://3.87.226.20:5005';
var state="off";
var myname="";
var inthandle;


/* Start with text input and status hidden */
document.getElementById('chatinput').style.display = 'none';
document.getElementById('status').style.display = 'none';
document.getElementById('leave').style.display = 'none';
// Action if they push the join button
document.getElementById('login-btn').addEventListener("click", (e) => {
	join();
})

document.getElementById('save-btn').addEventListener("click", (e) => {
	register();
})

/* Set up buttons */
document.getElementById('leave-btn').addEventListener("click", leaveSession);
document.getElementById('send-btn').addEventListener("click", sendText);
document.getElementById('Delete-btn').addEventListener("click", deleteConvo);

/*Drop Down Buttons*/
document.querySelector("#Original-btn").addEventListener("click", (e) => {
    setOriginal();
});
document.querySelector("#Fire-btn").addEventListener("click", (e) => {
    setFire();
});
document.querySelector("#Nature-btn").addEventListener("click", (e) => {
    setNature();
});
/*	var box = document.getElementsByClassName('.scrollabletextbox');
	background = {};styles for box are showing up as undefined--probably because not an object
        width = {}; //attempt to declare glabally to see if error would be fixed.
        fontFamily= {};
        fontSize= {};
        overflow={}; */

function setOriginal() {
	document.body.style.background = "#00BFFF";
	var box = document.querySelectorAll('.scrollabletextbox');
	[].forEach.call(box, function(ele) {
	ele.style.background = "#66FFFF";/*styles for box are showing up as undefined*/
	ele.style.height = "300px";
        ele.style.width = "400px";
        ele.style.fontFamily= "Verdana, Tahoma, Arial, Helvetica, sans-serif";
        ele.style.fontSize= "82%";
        ele.style.overflow="scroll";		
});
}
function setFire() {
	document.body.style.background = "#ff3300";
	var box = document.querySelectorAll('.scrollabletextbox');
	[].forEach.call(box, function(ele) {
	ele.style.background="#ff9900";
	ele.style.height= "300px";
        ele.style.width= "400px";
        ele.style.fontFamily= "Verdana, Tahoma, Arial, Helvetica, sans-serif";
        ele.style.fontSize= "82%";
        ele.style.overflow="scroll";		
});


}


function setNature() {
	document.body.style.background = "#009933";
	var box = document.querySelectorAll('.scrollabletextbox');
	[].forEach.call(box, function(ele) {
	ele.style.background="#ccffcc";
	ele.style.height= "300px";
        ele.style.width= "400px";
        ele.style.fontFamily= "Verdana, Tahoma, Arial, Helvetica, sans-serif";
        ele.style.fontSize= "82%";
        ele.style.overflow="scroll";		
});
}
// Watch for enter on message box
document.getElementById('message').addEventListener("keydown", (e)=> {
    if (e.code == "Enter") {
	sendText();
    }   
});



// Call function on page exit
window.onbeforeunload = leaveSession;



function completeJoin(results) {
	var status = results['status'];
	if (status != "success") {
		alert("Please Register!");
		leaveSession();
		return;
	}
	var user = results['user'];
	console.log("Join:"+user);
	startSession(user);
}

function join() {
	console.log("joining")
	myname = document.getElementById('yourname').value;
	pass = document.getElementById('yourpass').value;
	fetch(baseUrl+'/chat/join/'+myname+'/'+pass, {
        method: 'get'
    })
    .then (response => response.json())
    .then (data =>completeJoin(data))
    .catch(error => {
        {alert("Error: Something went wrong:"+error);}
    })
}

function completereg(results) {
	var status = results['status'];
	if (status != "success") {
		alert("Reg fail!");
		leaveSession();
		return;
	}
	var user = results['status'];
	console.log("Reg:"+user);
//	startSession(user);
}

function register() {
	console.log("registering")
	myname = document.getElementById('orangeForm-name').value;
	email = document.getElementById('orangeForm-email').value;
	pass = document.getElementById('orangeForm-pass').value;
	fetch(baseUrl+'/chat/register/'+myname+'/'+email+'/'+pass, {
        method: 'get'
    })
    .then (response => response.json() )
    .then (data =>completereg(data))
    .catch(error => {
        {alert("Error: Something went wrong:"+error);}
    })
}

function completeSend(results) {
	var status = results['status'];
	if (status == "success") {
		console.log("Send succeeded")
	} else {
		alert("Error sending message!");
	}
}

//function called on submit or enter on text input
function sendText() {
    var message = document.getElementById('message').value;
    console.log("Send: "+myname+":"+message);
	fetch(baseUrl+'/chat/send/'+myname+'/'+message, {
        method: 'get'
    })
    .then (response => response.json() )
    .then (data =>completeSend(data))
    .catch(error => {
        {alert("Error: Something went wrong:"+error);}
    })    

}

var del =false;
function deleteConvo(){
 del =true; 
	//document.getElementById('chatBox').innerHTML ="";

console.log("Cleared chat");
}

function completeFetch(result) {
	messages = result["messages"];
	messages.forEach(function (m,i) {
		name = m['user'];
		message = m['message'];
  	var boxmess = "<font color='red'>" + name + ": </font>" + message + "<br />";
	document.getElementById('chatBox').innerHTML +=
	boxmess;
	//setInterval(function(){document.getElementById('chatBox').innerHTML-=boxmess;}, 20000);
	});

	if(del==true){

	document.getElementById('chatBox').innerHTML ="";
	del=false;
	}

	users = result["users"];
	 document.getElementById('members').innerHTML =
                users;
}



/* Check for new messages */
function fetchMessage() {
	fetch(baseUrl+'/chat/fetch/'+myname, {
        method: 'get'
    })
    .then (response => response.json())
    .then (data =>completeFetch(data))
  
//    .then (data =>userlist(data))
    .catch(error => {
        {console.log("Server appears down");}
    })  	

}
/* Functions to set up visibility of sections of the display */
function startSession(name){
    state="on";
    
    document.getElementById('yourname').value = "";
    document.getElementById('register').style.display = 'none';
    document.getElementById('user').innerHTML = "User: " + name;
    document.getElementById('chatinput').style.display = 'block';
    document.getElementById('status').style.display = 'block';
    document.getElementById('leave').style.display = 'block';
    //setInterval(function(){document.getElementById('chatBox').innerHTML="";}, 10000);

    /* Check for messages every 500 ms */
    inthandle=setInterval(fetchMessage,500);
}

function leaveSession(){
    state="off";
    
    document.getElementById('yourname').value = "";
    document.getElementById('register').style.display = 'block';
    document.getElementById('user').innerHTML = "";
    document.getElementById('chatinput').style.display = 'none';
    document.getElementById('status').style.display = 'none';
    document.getElementById('leave').style.display = 'none';
	clearInterval(inthandle);
}



