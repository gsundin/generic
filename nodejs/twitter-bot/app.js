//console.log("Hello World!");

var TwitterPackage = require('twitter');

var secret = {
  consumer_key: 'PUT YOURS',
  consumer_secret: 'PUT YOURS',
  access_token_key: 'PUT YOURS',
  access_token_secret: 'PUT YOURS'
}

var Twitter = new TwitterPackage(secret);

Twitter.post('statuses/update', {status: 'I Love Tech Knights!'},  function(error, tweet, response){
  if(error){
    console.log(error);
  }
  console.log(tweet);  // Tweet body.
  console.log(response);  // Raw response object.
});


//console.log("Goodbye World!");
