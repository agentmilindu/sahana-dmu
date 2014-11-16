<?php
/**
 * @name         Crisis Management System
 * @version      11
 * @package      cms
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
require_once($global['approot'] . '/mod/cms/xajax.inc');

shn_xajax_registerFunction('cms_create_new');
shn_xajax_registerFunction('cms_save_map');
shn_xajax_registerFunction('cms_update_map');
shn_xajax_registerFunction('cms_delete_map');

shn_xajax_registerFunction('cms_add_layer');
shn_xajax_registerFunction('cms_rename_layer');
shn_xajax_registerFunction('cms_delete_layer');
shn_xajax_registerFunction('cms_get_layer_data');
shn_xajax_registerFunction('cms_get_Vector_layer_data');

shn_xajax_registerFunction('cms_create_tile_layer');
shn_xajax_registerFunction('cms_create_image_layer');
shn_xajax_registerFunction('cms_create_vector_layer');
shn_xajax_registerFunction('cms_create_vector_layer_by_URL');
shn_xajax_registerFunction('cms_create_layer_by_importing');

shn_xajax_registerFunction('cms_Tile_layer_options');
shn_xajax_registerFunction('cms_vector_layer_options');
shn_xajax_registerFunction('cms_image_layer_options');
shn_xajax_registerFunction('cms_import_layer_options');
shn_xajax_registerFunction('cms_show_layers_of_map');

shn_xajax_registerFunction('cms_add_property');
shn_xajax_registerFunction('cms_update_property');
shn_xajax_registerFunction('cms_delete_proporty');

/** Displays the Resources pages' content on the front end of the site. */
function shn_cms_default()
{
    
    global $global;
    $control = "";
    
    shn_tabmenu_open();
    shn_tabmenu_item("maps", _t("CMS-Menu| Complains"), "cms");
    shn_tabmenu_item("add", _t("CMS-Menu| Crisis situations"), "cms");
    shn_tabmenu_item("add", _t("CMS-Menu| Crisis Reports"), "cms");
    shn_tabmenu_item("add", _t("CMS-Menu| Settings"), "cms");
    shn_tabmenu_close();
    
    if (isset($_GET['all'])) {
        
    } elseif (isset($_GET['add'])) {
        $control = "shn_cms_add_complain";
        
    }
    
    if ($control == "") {
        $control = "shn_cms_build";
    }
    
    $control();
    
}
function shn_cms_build()
{
    global $global;
    
    //echo "<style> " . file_get_contents($global['approot'] . "/mod/cms/main.css") . "</style>";
    echo "<style> div#complain {
border: 1px solid #DDD;
margin: 3px;
padding: 10px;
}</style>";
    $q      = "select * from map";
    $result = $global['db']->Execute($q);
    if ($result === false) {
        daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "rez pages header");
    }
    echo "<div id='paControl' class='controlArea'>

		<input class='styleTehButton' type='button' onclick='javascript: pa_append_log(\"Canceling Changes...<br>\"); setTimeout(\"pa_load_capacity();\", 250);' value='Add new complain'>
	</div>";
    if ($result) {
        // build page info
		echo "";
        while ($row = $result->FetchRow()) {
            //echo "<div class='map-item'onclick=\"javascript: window.location = '?add&tabid=1&map_id=" . $row['map_id'] . "&name=" . $row['name'] . "'\"><img src='" . $row['thumbnail'] . "'/><br/>" . $row['name'] . "</div>";
            echo "<div id='complain'><b>A complain</b><i> By Name At Hospital  </i> <p>Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.</p></div>";
        }
    }
	
	
    
}
function shn_text_cms_upload()
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
function shn_text_cms_image()
{
	if ($_FILES["file"]["error"] > 0) {
	  echo "{Error: " . $_FILES["file"]["error"] . "}";
	} else {
	  // echo "{ Upload: " . $_FILES["file"]["name"] . ",";
	  // echo "Type: " . $_FILES["file"]["type"] . ",";
	  // echo "Size: " . ($_FILES["file"]["size"] / 1024) . ",";
	  echo "" . $_FILES["file"]["name"]."";
	  $name = $_FILES["file"]["name"];
	  move_uploaded_file($_FILES["file"]["tmp_name"], "res/uploads/cms/$name");
	}

}
function shn_cms_add_complain()
{
    global $global;
    
    //echo "<style> " . file_get_contents($global['approot'] . "/mod/cms/main.css") . "</style>";
    
?>

<fieldset>
         <legend>Add new complain</legend>
         <label for="title">Title</label> <input type="text" id="title"> <br/>
         
         <div class="checkBox">
		 <label for="drill">Situation</label>
         <select id="drill">
         <option value="" selected="" disabled=""></option>
         <option value="1">Critical</option>
         <option value="2">Non-Critical</option>
         </select>
         </div>
		 <br/>
		 <label for="description">Description</label> <textarea id="description"></textarea> <br/>

         </fieldset>
		 
<?php
    //echo "<script> " . file_get_contents($global['approot'] . "/mod/cms/main.js") . "init_map($map_id);</script>";
    
}
function cms_read(){
	echo file_get_contents('tmp/map (5).geojson');
	echo "<br/>END";
	

}
