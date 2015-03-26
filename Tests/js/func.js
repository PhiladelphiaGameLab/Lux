function Test(name, URL, data){
	var request = new XMLHttpRequest();
	request.onreadystatechange=function(){
		if(request.readyState == 4 && request.status == 200){
			var placement = document.createElement("DIV");	
			placement.innerHTML = "<br/>"+name+" : ";
			try{
				var response = JSON.parse(request.responseText);
				if(response.status.code == 1){
					placement.innerHTML += "<div style='color:green'>"+JSON.stringify(response, null, '\t')+"</div><br/>";
				}else if(response.status.code == 0){
					placement.innerHTML += "<div style='color:red'>"+JSON.stringify(response, null, '\t')+"</div><br/>";
				}else{
					placement.innerHTML += "'" + JSON.stringify(response.status.code, null, '\t') +"' <div style='color:yellow'>"+JSON.stringify(response, null, '\t')+"</div><br/>";
				}
			}catch(err){
				console.log("Can't Parse " + name + " because " + err);
				placement.innerHTML += "<div style='color:red'>"+request.responseText+"</div><br/>";
			}
			document.body.appendChild(placement);
		}
	}
	request.open("POST",URL,true);
	request.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
	request.send(JSON.stringify(data));
}

