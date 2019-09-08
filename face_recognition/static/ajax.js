$(function() {
    $.ajax({
        url: '/recognized_person',
        success: function(data) {
            document.getElementById("person_name").innerText = data['name'];
            document.getElementById("unlock").innerText = data['unlock'];
        }
    });
})