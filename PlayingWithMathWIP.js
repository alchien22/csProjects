function wordRanking(wordInput){
  var words=getColumn("Words", "Word");
  var length=getColumn("Words", "Length");
  for (var i=0; i<words.length; i++){
    if (words[i]==wordInput){
      return "The "+length[i]+" letter word, " +words[i]+", is the #"+(i+1)+" most common word in the English language.";
    }
  }
  return "Error. Not found.";
}

//console.log(wordRanking("the"));
//console.log(wordRanking("I"));
//console.log(wordRanking("for"));
//console.log(wordRanking("to"));

//how many multiples of 3 and 6 are there between 0 and 1000
//how many 4 digit numbers are also multiples of 4
function multiples(num, numRange, fullList){
  var fullSet=[];
  for (var a=0; a<(numRange[1]-numRange[0]); a++){
    if (numRange[0]!=numRange[1]){
      var x=numRange[0]+1;
      appendItem(fullSet, x);
    }
  }
  var newSet=[];
  var count=0;
  for (var i=0; i<numRange.length; i++){
    if ((fullSet[i]>=numRange[0] && fullSet[i]<=numRange[1]) && fullSet[i]%num==0){
      appendItem(newSet, fullSet[i]);
      count=count+1;
    }
  }
   if (fullList=="yes" || fullList=="Yes"){
    return "There are "+count+" multiples of "+num+" in this list. Here are all of the multiples: "+newSet;
    }else
    return "There are "+count+" multiples of "+num+" in this list.";
}

console.log(multiples(5, (1, 100), "yes"));
