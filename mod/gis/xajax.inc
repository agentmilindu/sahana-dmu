<?php
/**
 * @name         Expiry Queue
 * @version      0.1
 * @package      eq
 * @author       Greg Miernicki <g@miernicki.com> <gregory.miernicki@nih.gov>
 * @about        Developed in whole or part by the U.S. National Library of Medicine and the Sahana Foundation
 * @link         https://pl.nlm.nih.gov/about
 * @link         http://sahanafoundation.org
 * @license	 http://www.gnu.org/licenses/lgpl-2.1.html GNU Lesser General Public License (LGPL)
 * @lastModified 2011.1102
 */

global $global;

// create an array as it does not exist previosuly and will make warnings
$global['xajax_functions'] = array();

// publicly register accessible xajax funtions
array_push($global['xajax_functions'],'gis_create_new');
array_push($global['xajax_functions'],'gis_save_map');
array_push($global['xajax_functions'],'gis_update_map');
array_push($global['xajax_functions'],'gis_delete_map');

array_push($global['xajax_functions'],'gis_add_layer');
array_push($global['xajax_functions'],'gis_rename_layer');
array_push($global['xajax_functions'],'gis_delete_layer');
array_push($global['xajax_functions'],'gis_get_layer_data');
array_push($global['xajax_functions'],'gis_get_Vector_layer_data');

array_push($global['xajax_functions'],'gis_create_tile_layer');
array_push($global['xajax_functions'],'gis_create_image_layer');
array_push($global['xajax_functions'],'gis_create_vector_layer');
array_push($global['xajax_functions'],'gis_create_vector_layer_by_URL');

array_push($global['xajax_functions'],'gis_Tile_layer_options');
array_push($global['xajax_functions'],'gis_vector_layer_options');
array_push($global['xajax_functions'],'gis_image_layer_options');

array_push($global['xajax_functions'],'gis_add_property');
array_push($global['xajax_functions'],'gis_update_property');
array_push($global['xajax_functions'],'gis_delete_proporty');



//Maps
function gis_create_new() {
	global $global;
	
	$global['xajax_res']->addAppend('rezLog','innerHTML',$message);
	$global['xajax_res']->addScript("");
	return $global['xajax_res']->getXML();
}
function gis_save_map($name) {
	global $global;
	$q = "
		INSERT INTO map(name,thumbnail)
		VALUES ('$name','res/img/a-map.png');
	";
	$result = $global['db']->Execute($q);
	if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Inserting map"); }
	$global['xajax_res']->addScript("window.location='?add&tabid=1&map_id=".mysql_insert_id()."&name=$name'");
	return $global['xajax_res']->getXML();
}
function gis_update_map($id, $newName) {
	global $global;
	$q = "
		UPDATE map SET name='$newName' WHERE map_id=$id;
	";
	$result = $global['db']->Execute($q);
	if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Inserting map"); }
	

	$global['xajax_res']->addScript("$('#map-name').val('$newName')");
	return $global['xajax_res']->getXML();
}
function gis_delete_map($id) {
	global $global;
	$q = "
		DELETE FROM map_has_layer WHERE map_map_id=$id;
	";
	$result = $global['db']->Execute($q);
	if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Delete map_has_layer"); }
	$q = "
		DELETE FROM map WHERE map_id=$id;
	";
	$result = $global['db']->Execute($q);
	if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Delete map_has_layer"); }
	
	$global['xajax_res']->addScript("window.location='?maps&tabid=0'");
	return $global['xajax_res']->getXML();
}

//layers

function gis_add_layer() {
	global $global;
	$output = '<table class="emTable" id="add-layer-options"><tbody> <tr> <td class="mainRowEven"><b style="line-height: 28px;">Add layer</b> <input type="button" style="float: right; margin-left:3px;" class="styleTehButton" onclick="cancle_add_layer();" value="Cancel"><input type="button" style="float: right;" class="styleTehButton" onclick="save_layer();" value="Save"></td></tr>
<tr><td>Name: <input type="text" id="name"/> </td></tr><tr><td>Type: <label for="Tile" > <input type="radio" name="layer-type" value="Tile" id="Tile" onclick="showAddTileLayerOptions()"/> Tile Layer</label><label for="Vector"> <input type="radio" name="layer-type" value="Vector" id="Vector" onclick="showAddVectorLayerOptions()"/> Vector Layer</label> <input type="radio" name="layer-type" value="Image" id="Image" onclick="showAddImageLayerOptions()"/> Image Layer</label><label for="Tile" > <input type="radio" name="layer-type" value="Import" id="Import" onclick="showImportLayerOptions()"/> Import Layer</label><div id="layer-options"></div></td></tr><tr><td></td></tr></tbody></table>';
	$global['xajax_res']->addAssign('add-layer','innerHTML',$output);
	$global['xajax_res']->addScript("");
	return $global['xajax_res']->getXML();
}
function gis_rename_layer($id, $newName) {
	global $global;
	$q = "
		UPDATE layer SET name='$newName' WHERE layer_id=$id;
	";
	$result = $global['db']->Execute($q);
	if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Inserting map"); }
	
	$global['xajax_res']->addScript("$('#add-layer').html('');$('#layers tr#$id .name').html('$newName');");
	return $global['xajax_res']->getXML();
}
function gis_delete_layer($id) {
	global $global;
	$q = "
		DELETE FROM map_has_layer WHERE layer_layer_id=$id;
	";
	$result = $global['db']->Execute($q);
	if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Delete map_has_layer"); }
	
	$global['xajax_res']->addScript("$('#layers tr#$id').html('');getLayer($id).setVisible(false);");
	return $global['xajax_res']->getXML();
}

function gis_create_tile_layer($map_id, $name, $tileService, $options){
	global $global;
	$q = "
		INSERT INTO layer (name, type, options)
		VALUES ('$name','0','$options');
	";
	$result = $global['db']->Execute($q);
	if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Inserting layer"); }
	$layer_id = $global['db'] -> Insert_ID();
	//daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Inserted layer ");
	$q = "
		INSERT INTO map_has_layer (map_map_id, layer_layer_id, owner)
		VALUES ($map_id,$layer_id,1);
	";
	$result = $global['db']->Execute($q);
	if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Inserting map_has_layer"); }
	$global['xajax_res']->addScript("add_tile_layer($layer_id, '$name', ' $options ');$('#add-layer-options').remove()");
	return $global['xajax_res']->getXML();

}
function gis_create_image_layer($map_id, $name, $options){
	global $global;
	$q = "
		INSERT INTO layer (name, type, options)
		VALUES ('$name','2','$options');
	";
	$result = $global['db']->Execute($q);
	if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Inserting layer"); }
	$layer_id = $global['db'] -> Insert_ID();
	//daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Inserted layer ");
	$q = "
		INSERT INTO map_has_layer (map_map_id, layer_layer_id, owner)
		VALUES ($map_id,$layer_id,1);
	";
	$result = $global['db']->Execute($q);
	if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Inserting map_has_layer"); }
	$global['xajax_res']->addScript("add_image_layer($layer_id, '$name', ' $options ');$('#add-layer-options').remove()");
	return $global['xajax_res']->getXML();

}
function gis_create_vector_layer_by_URL($map_id, $name, $options, $url ){
	return gis_create_vector_layer($map_id, $name, $options, file_get_contents('tmp/'.$url));
}
function gis_create_vector_layer($map_id, $name, $options, $GeoJSONString) {
	global $global;
	
	$GeoJSON = json_decode($GeoJSONString, true);
        daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, 'gis_create_vector_layer', $GeoJSONString." ".$GeoJSON['type']);
	
	
	$q = "
		INSERT INTO layer (name, type, options)
		VALUES ('$name','1','$options');
	";
	$result = $global['db']->Execute($q);
	if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Inserting layer"); }
	$layer_id = $global['db'] -> Insert_ID();
	//daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Inserted layer ");
	$q = "
		INSERT INTO map_has_layer (map_map_id, layer_layer_id, owner)
		VALUES ($map_id,$layer_id,1);
	";
	$result = $global['db']->Execute($q);
	if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Inserting map_has_layer"); }
	
	
	
	foreach ($GeoJSON['features'] as $feature) {
                $geometryCode = 0;
		if($feature['geometry']['type'] == 'Point'){
                    $geometryCode = 1;
                }
		else if($feature['geometry']['type'] == 'LineString'){
			$geometryCode = 2;
		}
		else if($feature['geometry']['type'] == 'Polygon'){
			$geometryCode = 3;
		}
		$q = "
			INSERT INTO feature (geometry, layer_layer_id)
			VALUES ($geometryCode, $layer_id );
		";
		$result = $global['db']->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Inserting feature"); }
		$feature_id = $global['db'] -> Insert_ID();
                
                if($feature['geometry']['type'] == 'Point'){
                    $lat = $feature['geometry']['coordinates'][0];
                    $lon = $feature['geometry']['coordinates'][1];
                    $alt = $feature['geometry']['coordinates'][2]?$feature['geometry']['coordinates'][2]:0;
                    $q = "
						INSERT INTO marker(lat, lon, alt, feature_feature_id)
						VALUES ($lat, $lon, $alt, $feature_id );
                    ";
                    $result = $global['db']->Execute($q);
                    if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Inserting feature"); }
		
                }
                else if($feature['geometry']['type'] == 'LineString'){
					$q = "
							INSERT INTO polyline(feature_feature_id)
							VALUES ( $feature_id );
					";
					$result = $global['db']->Execute($q);
					if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Inserting polyline"); }
					$polyline_id = $global['db'] -> Insert_ID();
                    foreach( $feature['geometry']['coordinates'] as $point){
						$lat = $point[0];
						$lon = $point[1];
						$alt = isset($point[2])?$point[2]:0;
						
						$q = "
							INSERT INTO point(lat, lon, alt, polyline_polyline_id)
							VALUES ( $lat, $lon, $alt, $polyline_id );
						";
						$result = $global['db']->Execute($q);
						if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Inserting point"); }
					
					}
                }
                else if($feature['geometry']['type'] == 'Polygon'){
                    $q = "
							INSERT INTO polyline(feature_feature_id)
							VALUES ( $feature_id );
					";
					$result = $global['db']->Execute($q);
					if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Inserting polyline"); }
					$polyline_id = $global['db'] -> Insert_ID();
                    foreach( $feature['geometry']['coordinates'][0] as $point){
						$lat = $point[0];
						$lon = $point[1];
						$alt = isset($point[2])?$point[2]:0;
						
						$q = "
							INSERT INTO point(lat, lon, alt, polyline_polyline_id)
							VALUES ( $lat, $lon, $alt, $polyline_id );
						";
						$result = $global['db']->Execute($q);
						if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Inserting point"); }
					
					}
                }
		
		foreach ($feature['properties']  as $key => $value) {

			$q = "
				INSERT INTO property (name, value, feature_feature_id)
				VALUES ('$key','$value', $feature_id);
			";
			$result = $global['db']->Execute($q);
			if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Inserting properties of feature"); }
			
			
		}
                
                
                
	}
	//$global['xajax_res']->addAppend('rezLog','innerHTML',$message);
	//$global['xajax_res']->addScript("alert(".$layer_id.")");
	//return $global['xajax_res']->getXML();
	$global['xajax_res']->addScript("$('#add-layer').html('');");
	return gis_get_Vector_layer_data($name, $layer_id);
}
function gis_get_layer_data($map_id) {
	global $global;
	
	$q = "
		SELECT layer_id,name, type, options, owner
		FROM layer
		JOIN map_has_layer
		ON map_has_layer.layer_layer_id = layer.layer_id
		WHERE map_map_id = $map_id
		ORDER BY  type 
	";
	$result = $global['db']->Execute($q);
	if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "show events 1"); }
	$layers = Array();
	while($row = $result->FetchRow() ){
		Array_push(
			$layers,
			Array(
				"id" => $row['layer_id'],
				"name" => $row['name'],
				"type" => $row['type'],
				"options" => $row['options'],
				"owner" => $row['owner']
			)
		
		);
	}
	
	$global['xajax_res']->addScript("init_layers(".json_encode($layers).")");
	return $global['xajax_res']->getXML();
}

function gis_get_Vector_layer_data($name, $layer_id) {
	global $global;
	
	$q = "
		SELECT feature_id, geometry FROM feature
		WHERE layer_layer_id = $layer_id
	";
	$result = $global['db']->Execute($q);
	if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "show events 1"); }
	$layer = Array(
		"type" => "FeatureCollection",
		"features" => Array()
	);
	while($row = $result->FetchRow() ){
		$feature_id = $row['feature_id'];
		$q = "
			SELECT name, value FROM property
			WHERE feature_feature_id = $feature_id
		";
		$result2 = $global['db']->Execute($q);
		if($result2 === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "show events 1"); }
		$properties = new ArrayObject();
		while($row2 = $result2->FetchRow() ){
			$properties[$row2['name']] = $row2['value'];
		}
		
		$geometry = Array();
		
		if($row['geometry'] == "1"){
			$geometry = Array(
				"type" => "Point",
				"coordinates" => Array()
			);
			$q = "
			SELECT lat, lon, alt FROM marker
			WHERE feature_feature_id = $feature_id
			";
			$result3 = $global['db']->Execute($q);
			if($result3 === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "show events 1"); }
			while($row3 = $result3 -> FetchRow() ){
				Array_push(
					$geometry["coordinates"],
					(float) $row3['lat'],
					(float) $row3['lon'],
					(float) $row3['alt']
				);
			}
		}
		else if($row['geometry'] == "2"){
			$geometry = Array(
				"type" => "LineString",
				"coordinates" => Array()
			);
			$q = "
				SELECT lat, lon, alt FROM polyline
				JOIN point ON polyline_id = Polyline_polyline_id
				WHERE feature_feature_id =  $feature_id
			";
			$result3 = $global['db']->Execute($q);
			if($result3 === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "show events 1"); }
			while($row3 = $result3 -> FetchRow() ){
				Array_push(
					$geometry["coordinates"],
					Array(
					(float) $row3['lat'],
					(float) $row3['lon'],
					(float) $row3['alt']
					)
				);
			}
		}
		else if($row['geometry'] == "3"){
			$geometry = Array(
				"type" => "Polygon",
				"coordinates" => Array(
					Array()
				)
			);
			$q = "
				SELECT lat, lon, alt FROM polyline
				JOIN point ON polyline_id = Polyline_polyline_id
				WHERE feature_feature_id =  $feature_id
			";
			$result3 = $global['db']->Execute($q);
			if($result3 === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "show events 1"); }
			while($row3 = $result3 -> FetchRow() ){
				Array_push(
					$geometry["coordinates"][0],
					Array(
						
						(float) $row3['lat'],
						(float) $row3['lon'],
						(float) $row3['alt']
							
					)	
				);
			}
		}
		
		Array_push(
			$layer["features"],
			Array(
				"id" => $row['feature_id'],
				"type" => "Feature",
				"properties" => $properties,
				"geometry" => $geometry
			)
		
		);
	}
	
	$global['xajax_res']->addScript("add_vector_layer($layer_id,'$name', ".json_encode($layer).")");
	return $global['xajax_res']->getXML();
}

function gis_Tile_layer_options(){
	global $global;
	$output = '<select onchange="" id="tile-service">
  <option value="osm">OpenStreetMap</option>
  <option value="bing">Bing</option>
</select><div id="tile-options"></div>';
	$global['xajax_res']->addAssign('layer-options','innerHTML',$output);
	return $global['xajax_res']->getXML();
}
function gis_vector_layer_options(){
	global $global;
	$output = '<br/>GeoJSON:<br/><textarea id="GeoJSON-data" style="width: 95%;"></textarea><br/><br/> Or: <input id="fileupload" type="file" name="file"><!-- The global progress bar --><div id="progress" class="progress"><div class="progress-bar progress-bar-success"></div></div>';
	//$global['xajax_res']->addIncludeScript('res/js/jquery.ui.widget.js');
	//$global['xajax_res']->addIncludeScript('res/js/jquery.fileupload.js');
	//$global['xajax_res']->addIncludeScript('../mod/gis/fileupload.js');
	$global['xajax_res']->addScript("
		$.getScript( 'res/js/jquery.ui.widget.js', function( data, textStatus, jqxhr ) {
			$.getScript( 'res/js/jquery.fileupload.js', function( data, textStatus, jqxhr ) {
				$('#fileupload').fileupload({
				url: 'index.php?stream=text&mod=gis&act=upload',
				dataType: 'text',
				done: function (e, data) {
					$('#add-layer-options #GeoJSON-data').hide();
					$('#add-layer-options').append('<input type=\"hidden\" value=\"'+data.result+'\" name=\"URL\" id=\"URL\" />');
					$.each(data.files, function (index, file) {
						$('#layer-options').html('File : <input type=\"text\" value=\"'+file.name+'\" name=\"fileName\" id=\"fileName\" disabled/>');
					
					});
				},progressall: function (e, data) {
					var progress = parseInt(data.loaded / data.total * 100, 10);
					$('#progress .progress-bar').css('width', progress + '%');
				}});
			});
		});
	");
	$global['xajax_res']->addAssign('layer-options','innerHTML',$output);
	return $global['xajax_res']->getXML();
}

function gis_image_layer_options(){
	global $global;
	$output = '<hr/><table id="image-options"><tr><td>Upload: <input type="file" id="fileupload" name="file"/> </td></tr>';
	$output .= '<tr><td>URL: <input type="text" id="url"/> </td></tr>';
	$output .= '<tr><td>Projection: <select id="proj" > <option value="EPSG:3857">EPSG:3857</option><option value="EPSG:4326">EPSG:4326</option></select> </td></tr>';
	$output .= '<tr><td>Top-left long: <input type="text" id="tlln"/> </td></tr>';
	$output .= '<tr><td>Top-left lat: <input type="text" id="tllt"/> </td></tr>';
	$output .= '<tr><td>Bottom-right long: <input type="text" id="brln"/> </td></tr>';
	$output .= '<tr><td>Bottom-right lat: <input type="text" id="brlt"/> </td></tr>';
	$output .= '<tr><td>Height: <input type="text" id="h"/> </td></tr>';
	$output .= '<tr><td>Width : <input type="text" id="w"/> </td></tr></table>';
	$global['xajax_res']->addScript("
		$.getScript( 'res/js/jquery.ui.widget.js', function( data, textStatus, jqxhr ) {
			$.getScript( 'res/js/jquery.fileupload.js', function( data, textStatus, jqxhr ) {
				$('#fileupload').fileupload({
				url: 'index.php?stream=text&mod=gis&act=image',
				dataType: 'text',
				done: function (e, data) {
					$('#image-options').prepend('<input type=\"hidden\" value=\"'+data.result+'\" name=\"URL\" id=\"URL\" />');
					$('#image-options #url').val('res/uploads/gis/'+data.result);
				},progressall: function (e, data) {
					var progress = parseInt(data.loaded / data.total * 100, 10);
					$('#progress .progress-bar').css('width', progress + '%');
				}});
			});
		});
	");
	$global['xajax_res']->addAssign('layer-options','innerHTML',$output);
	return $global['xajax_res']->getXML();
}

//property

function gis_add_property($feature_id, $name, $value){
	global $global;
	
	$q = "
		INSERT INTO property(name, value, feature_feature_id)
		VALUES('$name', '$value' , $feature_id)
	";
	$result = $global['db']->Execute($q);
	if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "INSERT a Property"); }
				
			
	$global['xajax_res']->addScript("update_feature_property($feature_id, '".$name."', '".$value."')");
	return $global['xajax_res']->getXML();
}
function gis_update_property($feature_id, $oldName, $oldValue, $name, $value){
	global $global;
	
	$q = "
		UPDATE property SET name='$name', value='$value' WHERE feature_feature_id=$feature_id AND name='$oldName' AND value='$oldValue';
	";
	$result = $global['db']->Execute($q);
	if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Update a Property"); }
					
	$global['xajax_res']->addScript("update_property_changes($feature_id, '".$oldName."', '".$oldValue."', '".$name."', '".$value."')");
	return $global['xajax_res']->getXML();
}
function gis_delete_proporty($feature_id,  $name, $value){
	global $global;
	$q = "
		DELETE FROM  property WHERE feature_feature_id=$feature_id AND name='$name' AND value='$value';
	";
	$result = $global['db']->Execute($q);
	if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Update a Property"); }
					
	$global['xajax_res']->addScript("delete_property_changes($feature_id, '".$name."', '".$value."')");
	return $global['xajax_res']->getXML();
}




