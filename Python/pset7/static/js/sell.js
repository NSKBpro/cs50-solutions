jQuery(document).ready(function($){
        //Attach form submit listener
        $('form').submit(function(e){
            //Prevent default form submit action
            e.preventDefault();
            //Css display:block on loader div 
            $('#loader').show();
            //Send ajax request to backend
            $.ajax({
            url: "/sell",
            type: "POST",
            data:{
                symbol:$('#symbol').val(),
                shares:$('#shares').val()
            },
            success: function(result){
                $('#loader').hide();
                if(result.success == true){
                    //Show fancy sweetalert
                    swal("Good job", "Share " + $('#symbol').val() + " has been sold", "success");
                    //Reset input control value to empty
                    $('#shares').val('')
                }else{
                    //Show fancy sweetalert
                    swal("Oh no!", result.message, "error");
                    //Reset input control value to empty
                    $('#shares').val('')
                }
            },
            error: function(result){
                $('#loader').hide();
                swal("Oh no!", "Error while sending transaction to server.", "error");
            }
                
            });
        });
    });