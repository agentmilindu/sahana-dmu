<?php
/**
 * @name         Report Generation System
 * @version      11
 * @package      reporting
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
require_once($global['approot'] . '/mod/reporting/xajax.inc');

shn_xajax_registerFunction('reporting_create_new');


/** Displays the Resources pages' content on the front end of the site. */
function shn_reporting_default()
{
    
    global $global;
    $control = "";
    
    if ($control == "test") {
        require_once($global['approot'] . '/inc/tcpdf_include.php');
    }
	else if ($control == "") {
        $control = "shn_reporting_build";
		shn_tabmenu_open();
		shn_tabmenu_item("maps", _t("Reporting-Menu| Maps"), "reporting");
		shn_tabmenu_item("add", _t("Reporting-Menu| Map editor"), "reporting");
		shn_tabmenu_close();
    }
    
    $control();
    
}
function shn_reporting_build()
{
    
    global $global;
    
    echo "<a href='test'> New report </a>";
    
    
}