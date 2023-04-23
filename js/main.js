var b = 15;
			
function GetSwitchState(){
nocache = "&nocache=" + Math.random() * 1000000;
var request = new XMLHttpRequest();
    request.onreadystatechange = function(){
        if (this.readyState == 4) {
            if (this.status == 200) {
                if (this.responseText != null) {
                b = parseInt(this.responseText);
                }
            }
        }
    }
request.open("GET", "ajax_switch" + nocache, true);
request.send(null);
setTimeout('GetSwitchState()', 1000);
}



let secondHand=document.querySelector("#sec");  
let minHand=document.querySelector("#min")  
let hourHand=document.querySelector("#hr")  


setInterval(clockRotating,1000)  

function clockRotating(){ 

var date=new Date();  
var getqiblah=b;



minHand.style.transform="rotate("+b + "deg)"  


} 