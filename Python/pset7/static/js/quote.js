jQuery(document).ready(function($){
        //Attach form submit listener
        $('form').submit(function(e){
            //Prevent default form submit action
            e.preventDefault();
            //Css display:block on loader div 
            $('#loader').show();
            //Send ajax request to backend
            $.ajax({
            url: "/quote",
            type: "POST",
            data:{
                symbol:$('#symbol').val()
            },
            success: function(result){
                var prepareBody = "A share of " + result.name + "("+ result.symbol + ") costs " + result.price;
                $('#loader').hide();
                //Show fancy sweetalert
                swal(result.name, prepareBody, "success");
                //Reset input control value to empty
                $('#symbol').val('')
            },
            error: function(){
                $('#loader').hide();
                swal("Oh no!", "Error while getting quote information.", "error");
            }
                
            });
        });
    });