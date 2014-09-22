<?php
/**
 * @name         Disaster Assessment and Response
 * @version      01
 * @package      pa
 * @author       Ramindu Deshapriya <rdeshapriya@virtusa.com>
 * @about        Developed in whole or part by the U.S. National Library of Medicine,the Sahana Foundation and Virtusa Corpodarion
 * @link         https://pl.nlm.nih.gov/about
 * @link         http://sahanafoundation.org
 * @link         http://www.virtusa.com
 * @license         http://www.gnu.org/licenses/lgpl-2.1.html GNU Lesser General Public License (LGPL)
 * @lastModified 2013.11.21
 */

require_once($global['approot']."/inc/lib_form_return.inc");
global $global;

// create an array as it does not exist previosuly and will make warnings
$global['xajax_functions'] = array();

// publicly register accessible xajax funtions
array_push($global['xajax_functions'], 'pa_append_log');
array_push($global['xajax_functions'], 'pa_prepend_log');
array_push($global['xajax_functions'], 'pa_show_message');
array_push($global['xajax_functions'], 'pa_load_capacity');
array_push($global['xajax_functions'], 'pa_load_hazard');
array_push($global['xajax_functions'], 'pa_perform_save');
array_push($global['xajax_functions'], 'pa_hazard_history');

// adds a message to the log div @ tail
function pa_append_log($message = "no message specified?")
{

    global $global;
    //$global['xajax_res']->addAppend('paLog', 'innerHTML', $message);
    //---- scroll the log to the bottom
    //$global['xajax_res']->addScript("setTimeout('e = document.getElementById(\'paLog\'); e.scrollTop = e.scrollHeight+1000;', 500);");
    return $global['xajax_res']->getXML();
}


// adds a message to the log div @ head
function pa_prepend_log($message = "no message specified?")
{

    global $global;
    //$global['xajax_res']->addPrepend('paLog', 'innerHTML', $message);
    return $global['xajax_res']->getXML();
}


/**
 * shows a status message before another ajax function is executed
 */
function pa_show_message($message = "no message specified?")
{

    global $global;
    $global['xajax_res']->addAssign('paMain', 'innerHTML', '<div class="loadingMessage"><center><blink>' . $message . '</blink></center></div>');
    return $global['xajax_res']->getXML();
}

/**
 * Function to show a list of current reports
 */
/**function pa_show_reports() {
    global $global;
    $htmlLog = "";
    $htmlMain = "";
    $htmlMain .= "
			<table class=\"emTable\">
			<tr>
			<td style=\"font-size: 120%; padding-left: 8px;\" ><b>Date of Report</b></td>
			<td style=\"font-size: 120%; padding-left: 8px;text-align: center;\" ><b>UUID</b></td>
			<td style=\"font-size: 120%; padding-left: 8px;text-align: center;\" ><b>Disaster</b></td>
			<td style=\"font-size: 120%; padding-left: 8px;text-align: center;\" ><b>Assessor</b></td>
			<td style=\"font-size: 120%; padding-left: 8px;text-align: center;\" ><b>View/Edit</b></td>
			</tr>
			";

    $q = "
			SELECT  d.disaster_name, dar.dar_id, dar.date_created, dg.assessor_name
			FROM disaster AS d, disaster_assessment AS dar, dar_general AS dg
			WHERE d.disaster_id = dar.disaster_id
			AND dar.dar_id = dg.dar_id
			";
    $result = $global['db']->Execute($q);
    if($result === false) {
        daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "show reports");
    }
    $rcount = 0;
    $reports = array();
    while ( $row = $result->FetchRow() ) {
        $reports[$rcount] = $row;
        $rcount++;
    }
    $eo = 0;
    for ( $i = 0; $i < count($reports); $i++ ) {
        $row		=	$reports[$i];
        $uuid		=	$row['p_uuid'];
        $name		=	'<b>'.$row['full_name'].'</b>';
        $disease	=	$row['name'];

        if(($eo % 2) == 0) {
            $evenOddClass = "mainRowEven";
        } else {
            $evenOddClass = "mainRowOdd";
        }

        $htmlMain .= "
				<tr>
				<td style=\"padding-left: 8px;\" class=\"".$evenOddClass."\">".$name."</td>
				<td style=\"padding-left: 8px; text-align: center;\" class=\"".$evenOddClass."\"><a href=\"http://{$uuid}\">".$uuid."</a></td>
				<td style=\"padding-left: 8px; text-align: center;\" class=\"".$evenOddClass."\">".$disease."</td>
				<td style=\"padding-left: 8px; text-align: center;\" class=\"".$evenOddClass."\"><input class=\"styleTehButton\" type=\"button\" onclick=\"javascript: dar_append_log('Showing monitoring data for patient <b>".$name." </b>...<br>'); pa_show_list_for_patient('".$uuid."');\" value=\"View\"></td>
				<td style=\"padding-left: 8px; text-align: center;\" class=\"".$evenOddClass."\"><input class=\"styleTehButton\" type=\"button\" onclick=\"javascript: pa_append_log('Adding Monitoring data for patient <b>".$name." </b>...<br>'); pa_add_data('".$uuid."');\" value=\"Add\"></td>
				</tr>
				";
        $eo++;
    }
    $htmlMain .= "</table>";
    $pageControlHtml = '<input class="styleTehButton" type="button" onclick="javascript: pa_append_log(\''._t('Canceling Changes...').'<br>\'); setTimeout(\'pa_load_general();\', 250);" value="'._t('Add new Data').'">';
    $global['xajax_res']->addAssign('paMain','style.opacity','1.0');
    $global['xajax_res']->addAssign('paMain','innerHTML',$htmlMain);
    $global['xajax_res']->addAssign('paControl','style.opacity','1.0');
    $global['xajax_res']->addAssign('paControl','innerHTML',$pageControlHtml);
    $htmlLog .= _t("Found <b>").$rcount._t("</b> Report(s).<br>");
    $global['xajax_res']->addAppend('paLog','innerHTML',$htmlLog);
    $global['xajax_res']->addScript("setTimeout('e = document.getElementById(\'paLog \'); e.scrollTop = e.scrollHeight+1000;', 500);");
    return $global['xajax_res']->getXML();
}**/
/**
 * Function to load the General Information section
 */

function pa_load_capacity(){

    global $global;
    //$pa_id = $global['pa_id'];
    $htmlLog = "";
    $htmlMain = "";
    $_SESSION['pa_current_page'] = "loadCap";
    $htmlMain .= shn_form_fopen_return("", null, array('enctype' => 'enctype="multipart/form-data"', 'req_message' => true));
    $htmlMain .= shn_form_fsopen_return("Capacity Assessment");

    $htmlMain .=
       '<fieldset>
        <legend>Updating disaster data and related information in the system</legend>
        <form id="marks">
        <label>Select One</label>
        <select name="update_dis_data">
        <option value="" selected disabled></option>
        <option value="1"> Yes
        <option value="2"> No
        <option value="3"> Difficult to assess
        </select>
        </form>

        </fieldset>';
    $htmlMain .=
        '<fieldset>
         <legend>Public Awareness</legend>
         <label>Identifying possible risk groups</label>
         <div class="checkBox">
         <select id="pA" name="pA">
         <option value="" selected disabled></option>
         <option value="1">Yes</option>
         <option value="2">No</option>
         </select>
         <br><br>
         <fieldset id="pA_field" style="display:none">
         <label id="pa">Date/s</label><input type="text" id="pa_date"><br>
         <label id="pa">Place/s</label><input type="text" id="pa_Partially complete"><br>
         <div class="checkBox">
         <input type="checkbox" id="pa_not_plan">Not Planned
         </div>
         </fieldset>

         </fieldset>';

    $htmlMain .=
        '<fieldset>
         <legend>Mechanism for community-based early warning system</legend>
         <label>Mechanism in place</label>
         <div class="checkBox">
         <select id="eW" name="eW">
         <option value="" selected disabled></option>
         <option value="1">Yes</option>
         <option value="2">No</option>
         </select>
         <br><br>
         </div>
         <fieldset id="eW_field" style="display: none">
         <select id="eW2">
         <option value="1">Complete</option>
         <option value="2">Partial or non-functional</option>
         </select>
         </fieldset>

         </fieldset>';

    $htmlMain .=
        '<fieldset>
         <legend>Management of Self areas</legend>
         <label>Identification</label>
         <div class="checkBox">
         <select id="sA">
         <option value="" selected disabled></option>
         <option value="1">Yes</option>
         <option value="2">No</option>
         </select>
         <br><br>
         </div>
         <fieldset id="sA_field" style="display: none">
         <label>Mapping</label>
         <select id="sA2">
         <option value="" selected disabled></option>
         <option value="1">Satisfactory</option>
         <option value="2">Unsatisfactory</option>
         </select>
         </fieldset>
         <label>GIS coordinates</label>
         <div class="checkBox">
         <select id="sA3">
         <option value="" selected disabled></option>
         <option value="1">Yes</option>
         <option value="2">No</option>
         </select>
         <br><br>
         </div>

         </fieldset>';

    $htmlMain .=
        '<fieldset>
         <legend>Disaster Management Staff</legend>
         <label id="emp">Number of employees</label>
         <select id="emp">
         <option value="" selected disabled></option>
         <option value="1">Adequate</option>
         <option value="2">Inadequate</option>
         </select>
         <br><br>
         <label id="emp">Training of Staff</label>
         <select id="emp2">
         <option value="" selected disabled></option>
         <option value="1">Satisfactory</option>
         <option value="2">Unsatisfactory</option>
         </select>
         </fieldset>';

    $htmlMain .=
        '<fieldset>
         <legend>Stockpiling of emergency relief materials</legend>
         <label>Select One</label>
         <select id="eRM">
         <option value="" selected disabled></option>
         <option value="1">Satisfactory</option>
         <option value="2">Unsatisfactory</option>
         </select>
         </fieldset>';

    $htmlMain .=
        '<fieldset>
         <legend>Conducting drills and rehearsals</legend>
         <label>Planned</label>
         <div class="checkBox">
         <select id="drill">
         <option value="" selected disabled></option>
         <option value="1">Yes</option>
         <option value="2">No</option>
         </select>
         <br><br>
         </div>
         <fieldset id="drill_field" style="display: none">
         <label id="drill">Date/s</label><input type="text" id="drill_date"><br><br>
         <div class="checkBox">
         <input type="checkbox" id="drill_not_plan">Not Planned
         </div>
         </fieldset>

         </fieldset>';

    $htmlMain .=
        '<fieldset>
         <legend>Disaster Preparedness Plan for the Fiscal Year (according to the format proposedby DPRD)</legend>
         <label>Complete</label>
         <div class="checkBox">
         <select id="plan">
         <option value="" selected disabled></option>
         <option value="1">Yes</option>
         <option value="2">No</option>
         </select>
         <br><br>
         </div>
         <fieldset id="plan_field" style="display: none">
         <label>Uploaded or emailed a copy to DPRD</label>
         <div class="checkBox">
         <select id="plan2">
         <option value="" selected disabled></option>
         <option value="1">Yes</option>
         <option value="2">No</option>
         </select>
         <br><br>
         </div>
         </fieldset>

         </fieldset>';

    $htmlMain .= shn_form_fsclose_return();


    $editControlHtml = '

		<input class="styleTehButton" type="button" onclick="javascript: pa_append_log(\''._t('Canceling Changes...').'<br>\'); setTimeout(\'pa_load_capacity();\', 250);" value="'._t('Cancel Edit / Close').'">
		<input class="styleTehButton" type="button" onclick="javascript: pa_append_log(\''._t('Saving Changes...').'<br>\'); pa_perform_save(\''.$pa_id.'\', pa_get_capacity_data(), \'loadCap\');" value="'._t('Save Changes').'">
		<input class="styleTehButton" type="button" onclick="javascript: pa_append_log(\''._t('Saving Changes and Closing...<br>').'\'); pa_perform_save(\''.$pa_id.'\', pa_get_capacity_data(), \'loadCap\'); " value="'._t('Save Changes and Close').'">
	';
    $global['xajax_res']->addAssign('paMain','style.opacity','1.0');
    $global['xajax_res']->addAssign('paMain','innerHTML',$htmlMain);
    $global['xajax_res']->addAssign('paControl', 'innerHTML', $editControlHtml);
    //$global['xajax_res']->addAppend('paLog','innerHTML',$htmlLog);
    //$global['xajax_res']->addScript("setTimeout('e = document.getElementById(\'paLog\'); e.scrollTop = e.scrollHeight+1000;', 500);");
    $global['xajax_res']->addScript("tempCheckBox()");
    return $global['xajax_res']->getXML();

}

function pa_load_hazard(){
    global $global;
    //$pa_id = $global['pa_id'];
    $htmlLog = "";
    $htmlMain = "";
    $_SESSION['pa_current_page'] = "loadHazard";
    $htmlMain .= shn_form_fopen_return("", null, array('enctype' => 'enctype="multipart/form-data"', 'req_message' => true));
    $htmlMain .= shn_form_fsopen_return("Hazard Assessment");

    

    $htmlMain .= '<table>
                    <tr>
                        <th> HAZARD </th>
                        <th> POTENTIAL DAMAGE </th>
                        <th> FREQUENCY </th>
                        <th> SECONDARY PROBLEMS </th>
                        <th> TOTAL SCORE </th>
                    </tr>
                    <tr>
                        <td> Flood </td>
                        <td>
                            <form>
                            <select name="flood_poten" id="flood_poten">
                                <option value="0">  </option>
                                <option value="1">1. No damage</option>
                                <option value="2">2. Minor damage</option>
                                <option value="3">3. Moderate damage</option>
                                <option value="4">4. Significant damage</option>
                                <option value="5">5. Destroyed or nearly destroyed</option>
                            </select>
                            </form>
                        </td>
                        <td>
                            <form>
                            <select name="flood_frq" id="flood_frq">
                                <option value="0">  </option>
                                <option value="1">1. Has not occurred in last 100 years </option>
                                <option value="2">2. Happens at least once every fifty years</option>
                                <option value="3">3. Happens at least once every ten years</option>
                                <option value="4">4. Happens at least once every five years</option>
                                <option value="5">5. Annual event, or more often</option>
                            </select>
                            </form>
                        </td>
                        <td>
                            <form>
                            <select name="flood_SP" id="flood_SP">
                                <option value="0">  </option>
                                <option value="1">1. No secondary effects or problems likely</option>
                                <option value="2">2. At least one secondary effect, short-term in nature</option>
                                <option value="3">3. Multiple secondary effect(s). May last 2 or 3 days</option>
                                <option value="4">4. Significant secondary effect(s). May last a week</option>
                                <option value="5">5. Significant secondary effects last more than a week</option>
                            </select>
                            </form>
                        </td>
                        <td id="t_flood"></td>
                    </tr>
                    <tr>
                        <td> Landslide </td>
                        <td>
                            <form>
                            <select name="landslide_poten" id="landslide_poten">
                                <option value="0">  </option>
                                <option value="1">1. No damage</option>
                                <option value="2">2. Minor damage</option>
                                <option value="3">3. Moderate damage</option>
                                <option value="4">4. Significant damage</option>
                                <option value="5">5. Destroyed or nearly destroyed</option>
                            </select>
                            </form>
                        </td>
                        <td>
                            <form>
                            <select name="landslide_frq" id="landslide_frq">
                                <option value="0">  </option>
                                <option value="1">1. Has not occurred in last 100 years </option>
                                <option value="2">2. Happens at least once every fifty years</option>
                                <option value="3">3. Happens at least once every ten years</option>
                                <option value="4">4. Happens at least once every five years</option>
                                <option value="5">5. Annual event, or more often</option>
                            </select>
                            </form>
                        </td>
                        <td>
                            <form>
                            <select name="landslide_SP" id="landslide_SP">
                                <option value="0">  </option>
                                <option value="1">1. No secondary effects or problems likely</option>
                                <option value="2">2. At least one secondary effect, short-term in nature</option>
                                <option value="3">3. Multiple secondary effect(s). May last 2 or 3 days</option>
                                <option value="4">4. Significant secondary effect(s). May last a week</option>
                                <option value="5">5. Significant secondary effects last more than a week</option>
                            </select>
                            </form>
                        </td>
                        <td id="t_landslide"></td>
                    </tr>
                    <tr>
                        <td> Drought </td>
                        <td>
                            <form>
                            <select name="drought_poten" id="drought_poten">
                                <option value="0">  </option>
                                <option value="1">1. No damage</option>
                                <option value="2">2. Minor damage</option>
                                <option value="3">3. Moderate damage</option>
                                <option value="4">4. Significant damage</option>
                                <option value="5">5. Destroyed or nearly destroyed</option>
                            </select>
                            </form>
                        </td>
                        <td>
                            <form>
                            <select name="drought_frq" id="drought_frq">
                                <option value="0">  </option>
                                <option value="1">1. Has not occurred in last 100 years </option>
                                <option value="2">2. Happens at least once every fifty years</option>
                                <option value="3">3. Happens at least once every ten years</option>
                                <option value="4">4. Happens at least once every five years</option>
                                <option value="5">5. Annual event, or more often</option>
                            </select>
                            </form>
                        </td>
                        <td>
                            <form>
                            <select name="drought_SP" id="drought_SP">
                                <option value="0">  </option>
                                <option value="1">1. No secondary effects or problems likely</option>
                                <option value="2">2. At least one secondary effect, short-term in nature</option>
                                <option value="3">3. Multiple secondary effect(s). May last 2 or 3 days</option>
                                <option value="4">4. Significant secondary effect(s). May last a week</option>
                                <option value="5">5. Significant secondary effects last more than a week</option>
                            </select>
                            </form>
                        </td>
                        <td id="t_drought"></td>
                    </tr>
                    <tr>
                        <td> Cyclones </td>
                        <td>
                            <form>
                            <select name="cyclones_poten" id="cyclones_poten">
                                <option value="0">  </option>
                                <option value="1">1. No damage</option>
                                <option value="2">2. Minor damage</option>
                                <option value="3">3. Moderate damage</option>
                                <option value="4">4. Significant damage</option>
                                <option value="5">5. Destroyed or nearly destroyed</option>
                            </select>
                            </form>
                        </td>
                        <td>
                            <form>
                            <select name="cyclones_frq" id="cyclones_frq">
                                <option value="0">  </option>
                                <option value="1">1. Has not occurred in last 100 years </option>
                                <option value="2">2. Happens at least once every fifty years</option>
                                <option value="3">3. Happens at least once every ten years</option>
                                <option value="4">4. Happens at least once every five years</option>
                                <option value="5">5. Annual event, or more often</option>
                            </select>
                            </form>
                        </td>
                        <td>
                            <form>
                            <select name="cyclones_SP" id="cyclones_SP">
                                <option value="0">  </option>
                                <option value="1">1. No secondary effects or problems likely</option>
                                <option value="2">2. At least one secondary effect, short-term in nature</option>
                                <option value="3">3. Multiple secondary effect(s). May last 2 or 3 days</option>
                                <option value="4">4. Significant secondary effect(s). May last a week</option>
                                <option value="5">5. Significant secondary effects last more than a week</option>
                            </select>
                            </form>
                        </td>
                        <td id="t_cyclones"></td>
                    </tr>
                    <tr>
                        <td> Tsunami </td>
                        <td>
                            <form>
                            <select name="tsunami_poten" id="tsunami_poten">
                                <option value="0">  </option>
                                <option value="1">1. No damage</option>
                                <option value="2">2. Minor damage</option>
                                <option value="3">3. Moderate damage</option>
                                <option value="4">4. Significant damage</option>
                                <option value="5">5. Destroyed or nearly destroyed</option>
                            </select>
                            </form>
                        </td>
                        <td>
                            <form>
                            <select name="tsunami_frq" id="tsunami_frq">
                                <option value="0">  </option>
                                <option value="1">1. Has not occurred in last 100 years </option>
                                <option value="2">2. Happens at least once every fifty years</option>
                                <option value="3">3. Happens at least once every ten years</option>
                                <option value="4">4. Happens at least once every five years</option>
                                <option value="5">5. Annual event, or more often</option>
                            </select>
                            </form>
                        </td>
                        <td>
                            <form>
                            <select name="tsunami_SP" id="tsunami_SP">
                                <option value="0">  </option>
                                <option value="1">1. No secondary effects or problems likely</option>
                                <option value="2">2. At least one secondary effect, short-term in nature</option>
                                <option value="3">3. Multiple secondary effect(s). May last 2 or 3 days</option>
                                <option value="4">4. Significant secondary effect(s). May last a week</option>
                                <option value="5">5. Significant secondary effects last more than a week</option>
                            </select>
                            </form>
                        </td>
                        <td id="t_tsunami"></td>
                    </tr>
                    </table>';

    $htmlMain .= shn_form_fsclose_return();

    $htmlMain .= '<button class="styleTehButton" type="button" onclick="pa_perform_save(get_hazard())">Save Changes</button>';
    $htmlMain .= '<button class="styleTehButton" type="button" onclick="pa_hazard_history()" id="hazard_history">History</button>';
    $htmlMain .= '<button class="styleTehButton" type="button" onclick="hide_history()" id="hazard_history">Hide history</button>';
    $htmlMain .= '<br></br>';
    $htmlMain .= '<div id="history"></div>';
    //$htmlMain .= '<button class="styleTehButton" type="button" >$properties['flood_poten']</button>';




    $global['xajax_res']->addAssign('paMain','style.opacity','1.0');
    $global['xajax_res']->addAssign('paMain','innerHTML',$htmlMain);
    //$global['xajax_res']->addAssign('paControl', 'innerHTML', $editControlHtml);
    //$global['xajax_res']->addAppend('paLog','innerHTML',$htmlLog);
    //$global['xajax_res']->addScript("setTimeout('e = document.getElementById(\'paLog\'); e.scrollTop = e.scrollHeight+1000;', 500);");
    $global['xajax_res']->addScript("cal();");
    $global['xajax_res']->addScript("getToday();");
    
    return $global['xajax_res']->getXML();
}



function pa_perform_save($rj) {

    global $global;

    $r = json_decode($rj,true);

        $today             =mysql_real_escape_string($r["today"]);
        $flood_poten       =mysql_real_escape_string($r["flood_poten"]);
        $flood_frq         =mysql_real_escape_string($r["flood_frq"]);
        $flood_SP          =mysql_real_escape_string($r["flood_SP"]);
        $landslide_poten   =mysql_real_escape_string($r["landslide_poten"]);
        $landslide_frq     =mysql_real_escape_string($r["landslide_frq"]);
        $landslide_SP      =mysql_real_escape_string($r["landslide_SP"]);
        $drought_poten     =mysql_real_escape_string($r["drought_poten"]);
        $drought_frq       =mysql_real_escape_string($r["drought_frq"]);
        $drought_SP        =mysql_real_escape_string($r["drought_SP"]);
        $cyclones_poten    =mysql_real_escape_string($r["cyclones_poten"]);
        $cyclones_frq      =mysql_real_escape_string($r["cyclones_frq"]);
        $cyclones_SP       =mysql_real_escape_string($r["cyclones_SP"]);
        $tsunami_poten     =mysql_real_escape_string($r["tsunami_poten"]);
        $tsunami_frq       =mysql_real_escape_string($r["tsunami_frq"]);
        $tsunami_SP        =mysql_real_escape_string($r["tsunami_SP"]);

        $q = "INSERT INTO pa_hazard VALUES('','".$today."','".$flood_poten."','".$flood_frq."','".$flood_SP."','".$landslide_poten."','".$landslide_frq."','".$landslide_SP."','".$drought_poten."','".$drought_frq."','".$drought_SP."','".$cyclones_poten."','".$cyclones_frq."','".$cyclones_SP."','".$tsunami_poten."','".$tsunami_frq."','".$tsunami_SP."')";
        

    $result = $global['db']->Execute($q);
    return $global['xajax_res']->getXML();


}

function pa_hazard_history(){

    global $global;

    $q = "SELECT * FROM pa_hazard";

    $marks =Array();

    $result = $global['db']->Execute($q);

    while($row = $result->FetchRow() ){
        Array_push(
            $marks,
            Array(
                "hazard_id"         => $row['hazard_id'],
                "hazard_date"       => $row['hazard_date'],
                "flood_poten"       => $row['flood_poten'],
                "flood_frq"         => $row['flood_frq'],
                "flood_SP"          => $row['flood_SP'],
                "landslide_poten"   => $row['landslide_poten'],
                "landslide_frq"     => $row['landslide_frq'],
                "landslide_SP"      => $row['landslide_SP'],
                "drought_poten"     => $row['drought_poten'],
                "drought_frq"       => $row['drought_frq'],
                "drought_SP"        => $row['drought_SP'],
                "cyclones_poten"    => $row['cyclones_poten'],
                "cyclones_frq"      => $row['cyclones_frq'],
                "cyclones_SP"       => $row['cyclones_SP'],
                "tsunami_poten"     => $row['tsunami_poten'],
                "tsunami_frq"       => $row['tsunami_frq'],
                "tsunami_SP"        => $row['tsunami_SP']
            )
        
        );
    }
    
    $global['xajax_res']->addScript("showHistory(".json_encode($marks).")");

    return $global['xajax_res']->getXML();

}






