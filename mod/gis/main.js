//init

app = {}

$(document).ready(function(){
	
		
	var dragAndDropInteraction = new ol.interaction.DragAndDrop({
	  formatConstructors: [
		ol.format.GPX,
		ol.format.GeoJSON,
		ol.format.IGC,
		ol.format.KML,
		ol.format.TopoJSON
	  ]
	});
	
	olmap = new ol.Map({
        target: 'the-map',
        layers: [],
		interactions: ol.interaction.defaults().extend([dragAndDropInteraction]),

        view: new ol.View({
          center: [0,0],
          zoom: 2
        })
      });
	  
	
	olmap.on('click', function(evt) {
		displayFeatureInfo(evt.pixel);
	});
	var highlightStyleCache = {};
	
	
	dragAndDropInteraction.on('addfeatures', function(event) {
		
	  var vectorSource = new ol.source.Vector({
		features: event.features,
		projection: event.projection
	  });
	  olmap.getLayers().push(new ol.layer.Vector({
		source: vectorSource
	  }));
	  var view2D = olmap.getView().getView2D();
	  view2D.fitExtent(vectorSource.getExtent(), olmap.getSize());
	});

	featureOverlay = new ol.FeatureOverlay({
	  map: olmap,
	  style: function(feature, resolution) {
		var text =  feature.getId();
		if (!highlightStyleCache[text]) {
		  highlightStyleCache[text] = [new ol.style.Style({
			stroke: new ol.style.Stroke({
			  color: '#f00',
			  width: 1
			}),
			fill: new ol.style.Fill({
			  color: 'rgba(255,0,0,0.1)'
			})
		  })];
		}
		return highlightStyleCache[text];
	  }
	});
	  
});	 

//Maps
function confirm_delete_map( map_id ){
	if(confirm("Are you sure?")){
		gis_delete_map(map_id)
	}
}
function save_map(){
	gis_save_map($('#map-name').val());
}

function init_map( map_id ){
	$(document).ready(function(){
		if(map_id){
			app.map_id = map_id;
			gis_get_layer_data(map_id);
		}
		
	});
}

//Layers
//---------------
//Initialize the layers of the map
function init_layers( data ){
	app.layers  = data;
	
	//alert(JSON.stringify(data));
	$(data).each(function(){

		if(this.type == "0" ){
			add_tile_layer(this.id, this.name, this.options );
		}
		else if(this.type == "1" ){
			gis_get_Vector_layer_data(this.name,this.id);
		}
	});
}

//Show the options for adding a layers to the map
function add_layer(){
	if(app.map_id){
		$('#add-layer').html('<p>Loading...</P>');
		gis_add_layer();
	}
	else{
		alert('Please, save the map before adding layers');
	}
}
//Show the options for renaming a layer
function show_rename_layer(id){

		$('#add-layer').html('<table class="emTable" id="rename-layer"> <tr> <td class="mainRowEven"><b style="line-height: 28px;">Rename Layer</b><input type="button" style="float: right;" class="styleTehButton" onclick="rename_layer('+id+', $(\'#rename-layer #name\').val());" value="Save"></td></tr><tr><td>New Name: <input type="text" id="name"/></td></tr></table>');

	
		
}
function rename_layer(id, newName){
	if(app.map_id){
		$('#add-layer').html('<p>Loading...</P>');
		gis_rename_layer(id, newName);
	}
	else{
		alert('Please, save the map before adding layers');
	}
}


//Save the layer 
function save_layer(){

	if($('#add-layer-options #Tile').is(':checked')){
		if($('#add-layer-options #name').val()){
			var tileService = $('#add-layer-options #name').val();
			var layerName = $('#add-layer-options #name').val();
			var options = '';
			if(tileService = 'osm' ){
			 options = '{ source: new ol.source.OSM() }';
			}
			gis_create_tile_layer(app.map_id, layerName, tileService, options);
		}else{
			alert('Please, enter a name for the layer!');
		}
	}
	else if($('#add-layer-options #Vector').is(':checked')){
		if($('#add-layer-options #name').val()){
			gis_create_vector_layer(app.map_id, $('#add-layer-options #name').val(), '', $('#add-layer-options #GeoJSON-data').val());
		}else{
			alert('Please, enter a name for the layer!');
		}
	}

	else if($('#add-layer-options #Image').is(':checked')){
		if($('#add-layer-options #name').val()){
			//gis_create_vector_layer(app.map_id, $('#add-layer-options #name').val(), '', $('#add-layer-options #GeoJSON-data').val());
			olmap.addLayer(new ol.layer.Image({
				source: new ol.source.ImageStatic({
						url: $('#add-layer-options #url').val(),
						imageSize: [193, 261],
						imageExtent:  [7938720, 953570, 8167236, 576900], 
						projection: 'EPSG:3857'
					})
				}));
		}else{
			alert('Please, enter a name for the layer!');
		}
	}

	else{
		alert('Please, select a layer type!')
	}
}

//function for adding GeoJSON data onto the OL3 map
function add_vector_layer(id,  name, data ){
	var jsonLayer = new ol.layer.Vector({
	 id: id,
	 name: name,
     source: new ol.source.GeoJSON({
        object: data,
        projection: 'EPSG:3857'
     })
  });
  		$('#layers').append("<tr> <td class='mainRow'><input type='checkbox' id='"+id+"' onclick='toggleVisibility("+id+")' checked/>"+name+" <span class='tools'> [ <a href='#' onclick='show_rename_layer("+id+")'>Rename </a> | <a href='#'>Delete</a> ] </span></td></tr>");
olmap.addLayer(jsonLayer);
}
//function for adding a tile layer onto to the OL3 map
function add_tile_layer(id,  name, data ){
	var layer = new ol.layer.Tile(  eval("( "+data+")") );
	$('#layers').append("<tr> <td class='mainRow'><input type='checkbox' id='"+id+"' onclick='toggleVisibility("+id+")' checked/>"+name+" <span class='tools'> [ <a href='#' onclick='show_rename_layer("+id+")'>Rename </a> | <a href='#'>Delete</a> ] </span></td></tr>");

	olmap.addLayer( layer );
}

//Cancel the adding layer;hides the add layer option.
function cancle_add_layer(){
	$('#add-layer').html('');
}

//Features
//----------------------

var highlight;
function displayFeatureInfo(pixel) {

  var feature = olmap.forEachFeatureAtPixel(pixel, function(feature, layer) {
    return feature;
  });

  var info = document.getElementById('properties-panel');
  if (feature) {
	theFeature = feature;
    info.innerHTML = '<table class="emTable" id="properties"> <tr> <td class="mainRowEven"><b style="line-height: 28px;">Properties</b><input type="button" style="float: right;" class="styleTehButton" onclick="show_add_property('+feature.getId()+');" value="Add Property"></td></tr></table>';
	$(feature.getKeys()).each(function(key, element){
		if( element != 'geometry'){
			$('#properties').append('<tr><td>' + element + ' : ' + feature.get(element) + ' <span class="tools"> [ <a href="#" onclick="show_edit_property('+feature.getId()+', \'' + element + '\',  \'' + feature.get(element) + '\')">Edit</a> | <a href="#">Remove</a> ] </span></td></tr>');
		}
	});
  } else {
    info.innerHTML = '&nbsp;';
  }

  if (feature !== highlight) {
    if (highlight) {
      featureOverlay.removeFeature(highlight);
    }
    if (feature) {
      featureOverlay.addFeature(feature);
    }
    highlight = feature;
  }

};


function showAddTileLayerOptions(){
    gis_Tile_layer_options();
}
function showAddVectorLayerOptions(){
    //$('#layer-options').html('<p>Loading...</P>');
	gis_vector_layer_options();
}

function showAddImageLayerOptions(){
	gis_image_layer_options();
}

//property

function show_add_property(featureId){
	$('#properties-panel').prepend('<table class="emTable" id="add-property"> <tbody><tr> <td class="mainRowEven"><b style="line-height: 28px;">New Property</b><input type="button" style="float: right;margin-left:3px;" class="styleTehButton" onclick="$(\'#add-property\').html(\'\')" value="Cancle"><input type="button" style="float: right;" class="styleTehButton" onclick="save_property('+featureId+');" value="Save Property"></td></tr><tr><td>name : <input type="text" name="name"/> </td></tr><tr><td>Value : <input type="text" name="value"/> </td></tr></tbody></table>');
}
function save_property(featureId){
	gis_add_property(featureId, $('#add-property [name=name]').val(), $('#add-property [name=value]').val());
}
function update_property(featureId){
	gis_update_property(featureId, $('#edit-property [name=name]').val(), $('#edit-property [name=value]').val());
}
function update_feature_property(featureId, name, value){
	$('#add-property').html('');
	$('#properties').append('<tr><td>'+name+' : '+value+'</td></tr>');
	var t= theFeature.getProperties();
	t[''+name]=value;
	theFeature.setProperties(t);
}
function show_edit_property(featureId, name, value){
   $('#properties-panel').prepend('<table class="emTable" id="add-property"> <tbody><tr> <td class="mainRowEven"><b style="line-height: 28px;">Edit Property</b><input type="button" style="float: right;margin-left:3px;" class="styleTehButton" onclick="$(\'#add-property\').html(\'\')" value="Cancle"><input type="button" style="float: right;" class="styleTehButton" onclick="update_property('+featureId+',\''+name+'\',\''+value+'\');" value="Save Property"></td></tr><tr><td>name : <input type="text" name="name"/> </td></tr><tr><td>Value : <input type="text" name="value"/> </td></tr></tbody></table>');
}

//Helper functions
function toggleVisibility(id){
   getLayer(id).setVisible($('#layers input#'+id).is(':checked'));
}
function getLayer(id){
   return olmap.getLayers().getArray().filter(function(layer) {
            return layer.get('id') === id;
        })[0];
}

function test(){
    
    gis_create_vector_layer(2, 'Central Hospitals', '', '{"type":"FeatureCollection","features":[{"type":"Feature","id":"LKA","properties":{"name":"Sri Lanka"},"geometry":{"type":"Polygon","coordinates":[[[81.787959,7.523055],[81.637322,6.481775],[81.21802,6.197141],[80.348357,5.96837],[79.872469,6.763463],[79.695167,8.200843],[80.147801,9.824078],[80.838818,9.268427],[81.304319,8.564206],[81.787959,7.523055            ]] ]}}]}');
}