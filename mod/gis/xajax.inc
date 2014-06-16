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
array_push($global['xajax_functions'],'gis_get_layer_data');
array_push($global['xajax_functions'],'gis_get_Vector_layer_data');
array_push($global['xajax_functions'],'gis_create_vector_layer');

function gis_create_new() {
	global $global;
	
	$global['xajax_res']->addAppend('rezLog','innerHTML',$message);
	$global['xajax_res']->addScript("");
	return $global['xajax_res']->getXML();
}
function gis_save_map() {
	global $global;
	
	$global['xajax_res']->addAppend('rezLog','innerHTML',$message);
	$global['xajax_res']->addScript("");
	return $global['xajax_res']->getXML();
}
function gis_update_map() {
	global $global;
	
	$global['xajax_res']->addAppend('rezLog','innerHTML',$message);
	$global['xajax_res']->addScript("");
	return $global['xajax_res']->getXML();
}
function gis_create_vector_layer($map_id, $name, $options, $GeoJSONString) {
	global $global;
	
	$GeoJSON = json_decode($GeoJSONString, true);
        //(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Inserting feature ".$GeoJSONString." ".$GeoJSON['type']);
	
	
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
					if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Inserting feature"); }
					$polyline_id = $global['db'] -> Insert_ID();
                    foreach( $feature['geometry']['coordinates'] as $point){
						$lat = $point[0];
						$lon = $point[1];
						$alt = $point[2]?$point[2]:0;
						
						$q = "
							INSERT INTO point(lat, lon, alt, polyline_polyline_id)
							VALUES ( $point[0], $point[1], $polyline_id );
						";
						$result = $global['db']->Execute($q);
						if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Inserting feature"); }
					
					}
                }
                else if($feature['geometry']['type'] == 'Polygon'){
                    $geometryCode = 3;
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
	$global['xajax_res']->addScript("alert(".$layer_id.")");
	return $global['xajax_res']->getXML();
}
function gis_get_layer_data($map_id) {
	global $global;
	
	$q = "
		SELECT layer_id,name, type, options, owner
		FROM layer
		JOIN map_has_layer
		ON map_has_layer.layer_layer_id = layer.layer_id
		WHERE map_map_id = $map_id
		ORDER BY  type DESC
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

function gis_get_Vector_layer_data($layer_id) {
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
	
	$global['xajax_res']->addScript("add_vector_layer(".json_encode($layer).")");
	return $global['xajax_res']->getXML();
}



