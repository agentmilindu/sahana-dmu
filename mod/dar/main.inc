<?php
/**
 * @name         Disaster Assessment and Response
 * @version      01
 * @package      dar
 * @author       Ramindu Deshapriya <rdeshapriya@virtusa.com>
 * @about        Developed in whole or part by the U.S. National Library of Medicine,the Sahana Foundation and Virtusa Corpodarion
 * @link         https://pl.nlm.nih.gov/about
 * @link         http://sahanafoundation.org
 * @link		 http://www.virtusa.com
 * @license	 	 http://www.gnu.org/licenses/lgpl-2.1.html GNU Lesser General Public License (LGPL)
 * @lastModified 2013.11.21
 */


require_once($global['approot'].'/inc/lib_menu.inc');
require_once($global['approot'].'/inc/handler_db.inc');
require_once($global['approot'].'/inc/lib_xajax.inc');
require_once($global['approot'].'/mod/dar/xajax.inc');

shn_xajax_registerFunction('dar_append_log');
shn_xajax_registerFunction('dar_prepend_log');
shn_xajax_registerFunction('dar_show_message');
//shn_xajax_registerFunction('dar_store_values');
shn_xajax_registerFunction('dar_load_general');
shn_xajax_registerFunction('dar_load_popres');
shn_xajax_registerFunction('dar_load_wash');
shn_xajax_registerFunction('dar_load_food');
shn_xajax_registerFunction('dar_load_shelter');
shn_xajax_registerFunction('dar_perform_save');
shn_xajax_registerFunction('dar_show_reports');



function shn_dar_default() {

	global $global;
    shn_tabmenu_open();
    shn_tabmenu_item("general", _t("dar-Menu|General Info"),                 "dar");
    shn_tabmenu_item("popRes",  _t("dar-Menu|Population and Resource Data"), "dar");
    /*shn_tabmenu_item("indicators", _t("dar-Menu|Indicators"),                "dar");*/
    shn_tabmenu_item("wash", _t("dar-Menu|WASH"),                 "dar");
    shn_tabmenu_item("food",   _t("dar-Menu|Food security and nutrition"),   "dar");
    shn_tabmenu_item("shelter",  _t("dar-Menu|Shelters"),          "dar");
    shn_tabmenu_close();
	shn_xajax_printJavascript();
	echo "
		<div id=\"darLog\" class=\"logArea\">Loading Disaster Assessment and Response Data...<br><noscript>THIS MODULE REQUIRES THAT YOU ACTIVATE JAVASCRIPT!</noscript></div>
		<div id=\"darControl\" class=\"controlArea\">&nbsp;</div>
		<div id=\"darMain\" class=\"mainArea\"></div>
		<script>".file_get_contents($global['approot']."/mod/dar/dar.js")."</script>
		<script type=\"text/javascript\" src=\"res/js/jquery-1.6.4.min.js\" ></script>
		<script type=\"text/javascript\" src=\"res/js/jquery-ui-datepicker.min.js\" ></script>
	";
    echo "<style>".file_get_contents($global['approot']."/mod/dar/dar.css")."</style>";
    if(isset($_GET['general'])) {
        $control = "dar_load_general";

    } elseif(isset($_GET['popRes'])) {
        $control = "dar_load_popres";

    } /*elseif(isset($_GET['indicators'])) {
        $control = "dar_load_indicators";

    } */elseif(isset($_GET['wash'])) {
        $control = "dar_load_wash";

    } elseif(isset($_GET['food'])) {
        $control = "dar_load_food";

    } elseif(isset($_GET['shelter'])) {
        $control = "dar_load_shelter";

    } elseif(isset($_GET['loadCap'])){
        $control = "dar_load_capacity";

    } elseif(isset($_GET['loadHazard'])){
        $control = "dar_load_hazard";
    }
    else {
        $control = "dar_show_reports";
    }
	echo "
        <script type=\"text/javascript\">".$control."();</script>
		<script type=\"text/javascript\" src=\"res/js/jquery-1.6.4.min.js\" ></script>
		<script type=\"text/javascript\" src=\"res/js/jquery-ui-autocomplete.min.js\" ></script>
		<script type=\"text/javascript\" src=\"res/js/jquery-ui-datepicker.min.js\" ></script>
	";
}















