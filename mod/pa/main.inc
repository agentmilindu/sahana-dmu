<?php
/**
 * @name         Preparedness Assessment and Response
 * @version      01
 * @package      pa
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
require_once($global['approot'].'/mod/pa/xajax.inc');

shn_xajax_registerFunction('pa_append_log');
shn_xajax_registerFunction('pa_prepend_log');
shn_xajax_registerFunction('pa_show_message');
shn_xajax_registerFunction('pa_load_capacity');
shn_xajax_registerFunction('pa_load_hazard');
shn_xajax_registerFunction('pa_perform_save');
shn_xajax_registerFunction('pa_hazard_history');




function shn_pa_default() {

	global $global;
    shn_tabmenu_open();
    shn_tabmenu_item("loadCap",  _t("pa-Menu|Capacity Assessment"),          "pa");
    shn_tabmenu_item("loadHazard",  _t("pa-Menu|Hazard Assessment"),          "pa");
    shn_tabmenu_close();
    shn_xajax_printJavascript();
    
	shn_xajax_printJavascript();
	echo "
		<!--<div id=\"paLog\" class=\"logArea\">Loading pa Data...<br><noscript>THIS MODULE REQUIRES THAT YOU ACTIVATE JAVASCRIPT!</noscript></div>-->
		<div id=\"paControl\" class=\"controlArea\">&nbsp;</div>
		<div id=\"paMain\" class=\"mainArea\"></div>
		<script>".file_get_contents($global['approot']."/mod/pa/pa.js")."</script>
		<script type=\"text/javascript\" src=\"res/js/jquery-1.6.4.min.js\" ></script>
		<script type=\"text/javascript\" src=\"res/js/jquery-ui-datepicker.min.js\" ></script>
	";
    echo "<style>".file_get_contents($global['approot']."/mod/pa/pa.css")."</style>";
    if(isset($_GET['loadCap'])){
        $control = "pa_load_capacity";

    } elseif(isset($_GET['loadHazard'])){
        $control = "pa_load_hazard";
    }
    else {
        $control = "pa_show_reports";
    }
	echo "
        <script type=\"text/javascript\">".$control."();</script>
		<script type=\"text/javascript\" src=\"res/js/jquery-1.6.4.min.js\" ></script>
		<script type=\"text/javascript\" src=\"res/js/jquery-ui-autocomplete.min.js\" ></script>
		<script type=\"text/javascript\" src=\"res/js/jquery-ui-datepicker.min.js\" ></script>
	";
}















