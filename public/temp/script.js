function checkInput() {
    var portionInput = document.getElementById("portionInput").value.trim();
    var submitButton = document.getElementById("submitButton");
    if (portionInput !== "") {
        submitButton.classList.remove("hidden");
    } else {
        submitButton.classList.add("hidden");
    }
}

function animateCard() {
    var card = document.querySelector('.card');
    card.classList.add('animate');
    setTimeout(function() {
        card.classList.remove('animate');
    }, 1000);
    document.getElementById("submitButton").classList.add("hidden");
    document.getElementById("animation").classList.remove("hidden");
}
