<?php
/**
 * @name         Geographic Information System
 * @version      11
 * @package      gis
 * @author       Milindu Sanoj Kumarage <agentmilindu@gmail.com>
 * @about        Developed in whole or part by the U.S. National Library of Medicine, the Sahana Foundation.
 * @link         http://sahanafoundation.org
 * @link		 http://www.agentmilindu.com
 * @link         https://pl.nlm.nih.gov/about
 * @license	 http://www.gnu.org/licenses/lgpl-2.1.html GNU Lesser General Public License (LGPL)
 * @lastModified 2014.06.07
 */


global $global;

require_once($global['approot'] . '/inc/handler_db.inc');
require_once($global['approot'] . '/inc/lib_xajax.inc');
require_once($global['approot'] . '/mod/gis/xajax.inc');

shn_xajax_registerFunction('gis_create_new');
shn_xajax_registerFunction('gis_save_map');
shn_xajax_registerFunction('gis_update_map');
shn_xajax_registerFunction('gis_delete_map');

shn_xajax_registerFunction('gis_add_layer');
shn_xajax_registerFunction('gis_rename_layer');
shn_xajax_registerFunction('gis_delete_layer');
shn_xajax_registerFunction('gis_get_layer_data');
shn_xajax_registerFunction('gis_get_Vector_layer_data');

shn_xajax_registerFunction('gis_create_tile_layer');
shn_xajax_registerFunction('gis_create_image_layer');
shn_xajax_registerFunction('gis_create_vector_layer');
shn_xajax_registerFunction('gis_create_vector_layer_by_URL');

shn_xajax_registerFunction('gis_Tile_layer_options');
shn_xajax_registerFunction('gis_vector_layer_options');
shn_xajax_registerFunction('gis_image_layer_options');
shn_xajax_registerFunction('gis_add_property');
shn_xajax_registerFunction('gis_update_property');
shn_xajax_registerFunction('gis_delete_proporty');

/** Displays the Resources pages' content on the front end of the site. */
function shn_gis_default()
{
    
    global $global;
    $control = "";
    
    shn_tabmenu_open();
    shn_tabmenu_item("maps", _t("GIS-Menu| Maps"), "gis");
    shn_tabmenu_item("add", _t("GIS-Menu| Map editor"), "gis");
    shn_tabmenu_close();
    
    if (isset($_GET['rez'])) {
        //$control = "shn_rez_adm_default";
       // require_once($global['approot'] . "mod/rez/admin.inc");
        
    } elseif (isset($_GET['add'])) {
        $control = "shn_gis_add_map";
        
    } else if (isset($_GET['layer_id'])) {
        $control = "gis_get_vector_layer_data_geojson(" . $_GET['layer_id'] . ")";
    }
	else if (isset($_GET['upload'])) {
        $control = "gis_file_upload";
    }
	else if (isset($_GET['image'])) {
        $control = "gis_file_image_upload";
    }
	else if (isset($_GET['read'])) {
        $control = "gis_read";
    }
    
    if ($control == "") {
        $control = "shn_gis_build";
    }
    
    $control();
    
}
function shn_gis_build()
{
    
    global $global;
    
    echo "<style> " . file_get_contents($global['approot'] . "/mod/gis/main.css") . "</style>";
    $q      = "select * from map";
    $result = $global['db']->Execute($q);
    if ($result === false) {
        daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "rez pages header");
    }
    echo "<div class='map-item' onclick=\"javascript: window.location = '?add&tabid=1'\"><img src='http://placehold.it/150x150'/><br/>Add new map</div>";
    if ($result) {
        // build page info
        while ($row = $result->FetchRow()) {
            echo "<div class='map-item'onclick=\"javascript: window.location = '?add&tabid=1&map_id=" . $row['map_id'] . "&name=" . $row['name'] . "'\"><img src='" . $row['thumbnail'] . "'/><br/>" . $row['name'] . "</div>";
        }
    }
	
	
    
}
function shn_text_gis_upload()
{
	if ($_FILES["file"]["error"] > 0) {
	  echo "{Error: " . $_FILES["file"]["error"] . "}";
	} else {
	  // echo "{ Upload: " . $_FILES["file"]["name"] . ",";
	  // echo "Type: " . $_FILES["file"]["type"] . ",";
	  // echo "Size: " . ($_FILES["file"]["size"] / 1024) . ",";
	  echo "" . $_FILES["file"]["tmp_name"]."";
	  $name = $_FILES["file"]["name"];
	  move_uploaded_file($_FILES["file"]["tmp_name"], "tmp/$name");
	}

}
function shn_text_gis_image()
{
	if ($_FILES["file"]["error"] > 0) {
	  echo "{Error: " . $_FILES["file"]["error"] . "}";
	} else {
	  // echo "{ Upload: " . $_FILES["file"]["name"] . ",";
	  // echo "Type: " . $_FILES["file"]["type"] . ",";
	  // echo "Size: " . ($_FILES["file"]["size"] / 1024) . ",";
	  echo "" . $_FILES["file"]["name"]."";
	  $name = $_FILES["file"]["name"];
	  move_uploaded_file($_FILES["file"]["tmp_name"], "res/uploads/gis/$name");
	}

}
function shn_gis_add_map()
{
    global $global;
    
    echo "<style> " . file_get_contents($global['approot'] . "/mod/gis/main.css") . "</style>";
    
?>
	<!-- loading resources from external sources just for testing, will replace them with local sources -->
	<script src="http://ol3js.org/en/master/build/ol.js" type="text/javascript"></script>
	<script type="text/javascript" src="res/js/jquery-1.6.4.min.js" ></script>
	
	<table class="emTable">
	<tbody><tr>
		<td class="mainRowEven" style=" border-bottom: none;" colspan="2">
			<label>Name: </label>
			<input id="map-name" type="text" class="ui-autocomplete-input" autocomplete="off" role="textbox" aria-autocomplete="list" aria-haspopup="true" value="<?= isset($_GET['name']) ? $_GET['name'] : ""; ?>"> &nbsp;  &nbsp; 
			<?php
    if (!isset($_GET['name'])) {
?>
				<input type="button" style="float: right;" class="styleTehButton" onclick="save_map();" value="Save map"/>
			<?php
    } else {
?>
			    &nbsp;  &nbsp;<input type="button" style="float: right;" class="styleTehButton" onclick="confirm_delete_map(<?= $_GET['map_id'] ?>);" value="Delete map">
				&nbsp;  &nbsp;<input type="button" style="float: right;margin-right:2px;" class="styleTehButton" onclick="confirm_rename_map();" value="Update map"/>
				
			<?php
    }
?>
		</td>
	</tr>


	<tr>
		<td class="" style="border-top: none; border-bottom: none;  padding: 0px;" colspan="2">
			<div id="the-map" class="map"></div>
		</td>
	</tr>

	<tr>		<td class="mainRowEven" style=" border-top: none;" colspan="2">
			<label>latitude: </label>
				<input id="latitude" type="text" value="0" readonly="">
			<label> &nbsp; longitude: </label>
				<input id="longitude" type="text" value="0" readonly="">
		</td>
	</tr>
</tbody></table>
<div id='properties-panel'></div>
<div id='add-layer'></div><table class='emTable' id='layers'><tbody> <tr> <td class='mainRowEven'><b style='line-height: 28px;'>Layers</b> <input type='button' style='float: right;' class='styleTehButton' onclick='add_layer();' value='Add layer'></td></tr>
</tbody></table>		
<?php
    $map_id = isset($_GET['name'])?$_GET['map_id']:'';
    echo "<script> " . file_get_contents($global['approot'] . "/mod/gis/main.js") . "init_map($map_id);</script>";
    
}
function gis_read(){
	echo file_get_contents('tmp/map (5).geojson');
	echo "<br/>END";
	

}
