function makeRequest(server, uid, query_params) {
    var xmlHttp = new XMLHttpRequest();
    var url = server + query_params + "&acc_token=" + uid + "&testing=true"; //remove testing?
    xmlHttp.open( "GET", url, false );
    xmlHttp.send( null );
    return JSON.parse(xmlHttp.responseText);
}

function encodeItemArray(name, arr) {
    var output = [];
    for(i = 0; i < arr.length; i++) {
        output.push(encodeURI(name + "[" + i + "]=" + arr[i]));
    }
    return "&" + output.join("&");
}

function transaction(server, uid, id1, subId0, subId1, itemsId0, itemsId1) {
    var trans_params = "id1=" + id1 + "&subId0=" + subId0 + "&subId1=" + subId1;
    if(!Array.isArray(itemsId0) && itemsId0 != null) {
        throw "itemsId0 must be array or null";
    }
    if(!Array.isArray(itemsId1) && itemsId1 != null) {
        throw "itemsId1 must be array or null";
    }
    if(Array.isArray(itemsId0) && itemsId0 != 0) {
        trans_params += encodeItemArray("itemsId0", itemsId0);
    }
    if(Array.isArray(itemsId1)  && itemsId1 != 0) {
        trans_params += encodeItemArray("itemsId1", itemsId1);
    }
    return makeRequest(server, uid, "method=transaction&" + trans_params);
}

function addUserAccount(server, uid) {
    return makeRequest(server, uid, "method=addUserAccount");
}

function removeUserAccount(server, uid) {
    return makeRequest(server, uid, "method=removeUserAccount");
}

function addSubaccount(server, uid, description) {
    return makeRequest(server, uid, "method=addSubaccount&desc=" + description);
}

function removeSubaccount(server, uid, subId) {
    return makeRequest(server, uid, "method=removeSubaccount&subId=" + subId);
}

function getTransactionHistory(server, uid) {
    return makeRequest(server, uid, "method=getTransactionHistory");
}

function getTransactionDetail(server, uid, transId) {
    return makeRequest(server, uid, "method=getTransactionDetail&transId=" + transId);
}

function getAccountInfo(server, uid) {
    return makeRequest(server, uid, "method=getAccountInfo");
}

function getSubaccountInfo(server, uid, subId) {
    return makeRequest(server, uid, "method=getSubaccountDetail&subId=" + subId);
}

function addItemToSubaccount(server, uid, subId, itemId) {
    return makeRequest(server, uid, "method=addItemToSubaccount&subId=" + subId + "&itemId=" + itemId);
}

function removeItemFromSubaccount(server, uid, subId, itemId) {
    return makeRequest(server, uid, "method=removeItemFromSubaccountl&subId=" + subId + "&itemId=" + itemId);
}