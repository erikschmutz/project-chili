const path = "localhost:9200";
const databaseIndex = 'project_chili';
const defaultType = "data_value";
const elasticsearch = require('elasticsearch');
var client;
var id = 0;


(function init() {
    client = new elasticsearch.Client({
        host: 'localhost:9200',
        log: 'trace'
    });
    createIndex(databaseIndex);
})()
function createIndex(name) {
    client.indices.create({
        index: name
    }, function (err, resp, status) {
        if (err) {
            console.log(err);
        } else {

        }
    });
}
function createPost(post) {
    client.index(post, function (err, resp, status) {
    });
}
function search() {
    client.search({
        index: databaseIndex,
        type: 'posts',
        q: 'Tags:Node.js'
    }).then(function (resp) {
        console.log(resp);
    }, function (err) {
        console.trace(err.message);
    });
}
function generateDefaultPost(data_value){
    console.log(data_value)
    createPost(generatePost(id++,defaultType,generateTags(data_value+ ", "+getDateTime()), getDateTime(), data_value));
}
function generatePost(id, type, tags, time, value) {
    return {
        index: databaseIndex,
        id: id,
        type: type,
        body: {
            index: id,
            tags: tags,
            post_date: new Date(),
            value: value
        }

    }

}
function generateTags(tags){
    const defaultTags = "data_value, measured_value";
    return defaultTags+", "+tags;

}
function getDateTime() {

    var date = new Date();

    var hour = date.getHours();
    hour = (hour < 10 ? "0" : "") + hour;

    var min  = date.getMinutes();
    min = (min < 10 ? "0" : "") + min;

    var sec  = date.getSeconds();
    sec = (sec < 10 ? "0" : "") + sec;

    var year = date.getFullYear();

    var month = date.getMonth() + 1;
    month = (month < 10 ? "0" : "") + month;

    var day  = date.getDate();
    day = (day < 10 ? "0" : "") + day;

    return year + ":" + month + ":" + day + ":" + hour + ":" + min + ":" + sec;

}

module.exports = {
    generateDefaultPost: data_value=>generateDefaultPost(data_value)
}


/*
CLEAR DB COMMAND-

*/