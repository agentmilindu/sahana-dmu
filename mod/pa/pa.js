

function tempCheckBox(){
    $("select").change(function(){

        if ($("#pA").val()=="1") {
            $('#pA_field').slideDown();
  
        } if ($("#pA").val()=="2"){
            $('#pA_field').slideUp();

        }
        if ($("#eW").val()=="1"){
            $('#eW_field').slideDown();
   
        }
        if ($("#eW").val()=="2"){
            $('#eW_field').slideUp();
    
        }
        if ($("#sA").val()=="1"){
            $('#sA_field').slideDown();
  
        }
        if ($("#sA").val()=="2"){
            $('#sA_field').slideUp();
    
        }
        if ($("#drill").val()=="1"){
            $('#drill_field').slideDown();
      
        }
        if ($("#drill").val()=="2"){
            $('#drill_field').slideUp();
         
        }
        if ($("#plan").val()=="1"){
            $('#plan_field').slideDown();

        }
        if ($("#plan").val()=="2"){
            $('#plan_field').slideUp();
 
        }

    });
}

function cal(){
    $('select').change(function(){t_score()});
}

function t_score(){
    var f_1 = document.getElementById("flood_poten").value;
    var f_2 = document.getElementById("flood_frq").value;
    var f_3 = document.getElementById("flood_SP").value;
    document.getElementById("t_flood").innerHTML = parseInt(f_1)*parseInt(f_2)*parseInt(f_3);

    var l_1 = document.getElementById("landslide_poten").value;
    var l_2 = document.getElementById("landslide_frq").value;
    var l_3 = document.getElementById("landslide_SP").value;
    document.getElementById("t_landslide").innerHTML = parseInt(l_1)*parseInt(l_2)*parseInt(l_3);

    var d_1 = document.getElementById("drought_poten").value;
    var d_2 = document.getElementById("drought_frq").value;
    var d_3 = document.getElementById("drought_SP").value;
    document.getElementById("t_drought").innerHTML = parseInt(d_1)*parseInt(d_2)*parseInt(d_3);

    var c_1 = document.getElementById("cyclones_poten").value;
    var c_2 = document.getElementById("cyclones_frq").value;
    var c_3 = document.getElementById("cyclones_SP").value;
    document.getElementById("t_cyclones").innerHTML = parseInt(c_1)*parseInt(c_2)*parseInt(c_3);

    var t_1 = document.getElementById("tsunami_poten").value;
    var t_2 = document.getElementById("tsunami_frq").value;
    var t_3 = document.getElementById("tsunami_SP").value;
    document.getElementById("t_tsunami").innerHTML = parseInt(t_1)*parseInt(t_2)*parseInt(t_3);

}


function get_hazard() {
    var r = new Object();
    r.today             = getToday();
    r.flood_poten       =$("#flood_poten").val();
    r.flood_frq         =$("#flood_frq").val();
    r.flood_SP          =$("#flood_SP").val();
    r.landslide_poten   =$("#landslide_poten").val();
    r.landslide_frq     =$("#landslide_frq").val();
    r.landslide_SP      =$("#landslide_SP").val();
    r.drought_poten     =$("#drought_poten").val();
    r.drought_frq       =$("#drought_frq").val();
    r.drought_SP        =$("#drought_SP").val();
    r.cyclones_poten    =$("#cyclones_poten").val();
    r.cyclones_frq      =$("#cyclones_frq").val();
    r.cyclones_SP       =$("#cyclones_SP").val();
    r.tsunami_poten     =$("#tsunami_poten").val();
    r.tsunami_frq       =$("#tsunami_frq").val();
    r.tsunami_SP        =$("#tsunami_SP").val();

    var rj = JSON.stringify(r);
    return(rj);

}

function getToday()
{
    var today = new Date();
    var dd = today.getDate();
    var mm = today.getMonth()+1; //January is 0!
    var yyyy = today.getFullYear();

    if(dd<10) {
        dd='0'+dd
    } 

    if(mm<10) {
        mm='0'+mm
    } 

    today = yyyy+'-'+mm+'-'+dd;
    return (today);
}

function showAlert(){
    alert("alert");
}

function showHistory(data){
    
    $('#history').empty();
    for (var i = 0; i < data.length; i++){
        $('#history').prepend('<div id="his" class="'+data[i]['hazard_id']+'" ><table border="1">\
                    <tr>\
                        <td> HAZARD </td>\
                        <td> POTENTIAL DAMAGE </td>\
                        <td> FREQUENCY </td>\
                        <td> SECONDARY PROBLEMS </td>\
                        <td> TOTAL SCORE </td>\
                    </tr>\
                    <tr>\
                        <td> Flood </td>\
                        <td>\
                            '+data[i]['flood_poten']+'\
                        </td>\
                        <td>\
                            '+data[i]['flood_frq']+'\
                        </td>\
                        <td>\
                            '+data[i]['flood_SP']+'\
                        </td>\
                            <td>'+parseInt(data[i]['flood_poten'])*parseInt(data[i]['flood_frq'])*parseInt(data[i]['flood_SP'])+'</td>\
                    </tr>\
                    <tr>\
                        <td> Landslide </td>\
                        <td>\
                            '+data[i]['landslide_poten']+'\
                        </td>\
                        <td>\
                            '+data[i]['landslide_frq']+'\
                        </td>\
                        <td>\
                            '+data[i]['landslide_SP']+'\
                        </td>\
                            <td>'+parseInt(data[i]['landslide_poten'])*parseInt(data[i]['landslide_frq'])*parseInt(data[i]['landslide_SP'])+'</td>\
                    </tr>\
                    <tr>\
                        <td> Drought </td>\
                        <td>\
                            '+data[i]['drought_poten']+'\
                        </td>\
                        <td>\
                            '+data[i]['drought_frq']+'\
                        </td>\
                        <td>\
                            '+data[i]['drought_SP']+'\
                        </td>\
                            <td>'+parseInt(data[i]['drought_poten'])*parseInt(data[i]['drought_frq'])*parseInt(data[i]['drought_SP'])+'</td>\
                    </tr>\
                    <tr>\
                        <td> Cyclones </td>\
                        <td>\
                            '+data[i]['cyclones_poten']+'\
                        </td>\
                        <td>\
                            '+data[i]['cyclones_frq']+'\
                        </td>\
                        <td>\
                            '+data[i]['cyclones_SP']+'\
                        </td>\
                            <td>'+parseInt(data[i]['cyclones_poten'])*parseInt(data[i]['cyclones_frq'])*parseInt(data[i]['cyclones_SP'])+'</td>\
                    </tr>\
                    <tr>\
                        <td> Tsunami </td>\
                        <td>\
                            '+data[i]['tsunami_poten']+'\
                        </td>\
                        <td>\
                            '+data[i]['tsunami_frq']+'\
                        </td>\
                        <td>\
                            '+data[i]['tsunami_SP']+'\
                        </td>\
                            <td>'+parseInt(data[i]['tsunami_poten'])*parseInt(data[i]['tsunami_frq'])*parseInt(data[i]['tsunami_SP'])+'</td>\
                    </tr>\
                    </table></div><br></br>');
        $('#history').prepend("<p><b>Hazard Assesment on : "+data[i]['hazard_date']+"<b></p>");

    }
     
      $('#history').prepend("<h3><b>Hazard history</b></h3>");

}

function hide_history(){
     $('#history').empty();
}

