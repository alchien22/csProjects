//Load data for each planet (length of days, the average temperature, and an image of the planet) 
//from the database called "Planets of our Solar System" provided by code.org in appLab.
var planet = getColumn("Planets of our Solar System", "Planet");
var dayLength = getColumn("Planets of our Solar System", "Length of day");
var temp = getColumn("Planets of our Solar System","Mean temperature");
var icon = getColumn("Planets of our Solar System", "Picture");

//Create new variables that will hold the filtered data for the length of days in hours and the average temperature of a given planet.
var filteredDayLength = [];
var filteredTemp = [];

//Obtain the name of the selected planet from the planetInput dropdown.
//Call the function statsRefresh and pass the parameter argument: planetMatch.
onEvent("find", "click", function(){
  var planetMatch=getText("planetInput");
  statsRefresh(planetMatch);
});

//The statsRefresh function will filter the dayLength and temp lists and add the filtered values to filteredDayLength and filteredTemp 
//respectively using the planet input that the user selected. 
//The corresponding image url for each planet will also be selected and the image of the planet will be shown on the iconOutput.   
function statsRefresh(planetName){
  filteredDayLength = [];
  filteredTemp = [];
  for (var i=0; i<planet.length; i++){
    if (planet[i] == planetName){
      appendItem(filteredDayLength, dayLength[i]);
      appendItem(filteredTemp, temp[i]);
      setImageURL("iconOutput", icon[i]);
    }
  }
  setText("dayOutput", filteredDayLength + " hours");
  setText("tempOutput", filteredTemp + "°C");
  
//Asks the user to select a planet if the Select Planet dropdown option is selected.
  if (planetName == "Select Planet"){
    setProperty("dayOutput", "text", "Please select a planet from the dropdown list.");
    setProperty("tempOutput", "text", "Please select a planet from the dropdown list.");
    setProperty("dayOutput", "font-size", 18);
    setProperty("tempOutput", "font-size", 18);
    hideElement("iconOutput");
  }
  else {
  showElement("iconOutput");
  setProperty("dayOutput", "font-size", 24);
  setProperty("tempOutput", "font-size", 24);
  }
}