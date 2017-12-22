jQuery(document).ready(function($){
    $( ".buymore" ).click(function() {
        var tr = $(this).closest("tr")
        var symbol = $(this).closest("tr").find(".symbol").text()
        var share = $(this).closest("tr").find(".share")
        var total = $(this).closest("tr").find(".total")
        $('#loader').show();
        $.ajax({
            url: "/buy",
            type: "POST",
            data:{
                symbol:symbol,
                shares:1
            },
            success: function(result){
                if(result.success == true){
                    $.get( "/?justTotal=1", function( data ) {
                        var getInt = parseInt(share.text()) + 1
                        if(getInt > 0)
                            share.text((getInt.toString()))
                        else
                            tr.remove()
                        total.text(result.total)
                        $("#sum").text(data.total)
                        $('#loader').hide();
                        //Show fancy sweetalert
                        swal("Good job", "Share " + symbol + " has been bought", "success");
                    });
                }else{
                    //Show fancy sweetalert
                    swal("Oh no!", result.message, "error");
                }
            },
            error: function(result){
                $('#loader').hide();
                swal("Oh no!", "Error while sending transaction to server.", "error");
            }
        });
    });
    $( ".sellmore" ).click(function() {
        var tr = $(this).closest("tr")
        var symbol = $(this).closest("tr").find(".symbol").text()
        var share = $(this).closest("tr").find(".share")
        var total = $(this).closest("tr").find(".total")
        $('#loader').show();
        $.ajax({
            url: "/sell",
            type: "POST",
            data:{
                symbol:symbol,
                shares:1
            },
            success: function(result){
                if(result.success == true){
                    $.get( "/?justTotal=1", function( data ) {
                        $("#sum").text(data.total)
                        $('#loader').hide();
                        var getInt = parseInt(share.text()) -1
                        if(getInt > 0)
                            share.text((getInt.toString()))
                        else
                            tr.remove()
                        total.text(result.total)
                        //Show fancy sweetalert
                        swal("Good job", "Share " + symbol + " has been sold", "success");
                    });
                    
                }else{
                    //Show fancy sweetalert
                    swal("Oh no!", result.message, "error");
                }
            },
            error: function(result){
                $('#loader').hide();
                swal("Oh no!", "Error while sending transaction to server.", "error");
            }
        });
        //Decrement amount of shares
        
    });
});