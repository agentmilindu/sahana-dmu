<?php
/**
 * @name         Report Generation System
 * @version      0.1
 * @package      reporting
 * @author       Milindu Sanoj Kumarage <agentmilindu@gmail.com>
 * @about        Developed in whole or part by the U.S. National Library of Medicine and the Sahana Foundation
 * @link         https://pl.nlm.nih.gov/about
 * @link         http://sahanafoundation.org
 * @link		 http://www.agentmilindu.com
 * @license	 http://www.gnu.org/licenses/lgpl-2.1.html GNU Lesser General Public License (LGPL)
 * @lastModified 2014.07.08
 */

global $global;

// create an array as it does not exist previosuly and will make warnings
$global['xajax_functions'] = array();

// publicly register accessible xajax funtions
array_push($global['xajax_functions'],'gis_create_new');




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
		VALUES ('$name','http://placehold.it/150x150');
	";
	$result = $global['db']->Execute($q);
	if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Inserting map"); }
	$global['xajax_res']->addScript("window.location='?add&tabid=1&map_id=".mysql_insert_id()."&name=$name'");
	return $global['xajax_res']->getXML();
}