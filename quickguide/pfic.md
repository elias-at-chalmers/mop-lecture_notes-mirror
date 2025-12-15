<p>Short text about PFIC</p>

<p>
<details>
<summary>Base addresses</summary>

{{python quickguide-generator/main.py baseaddress PFIC*}}

</details>

<details open>
<summary>
Register Block Overview
</summary>

{{python quickguide-generator/main.py overview-table PFIC}}

</details>

<p>
<hr>

{{python quickguide-generator/main.py register-details PFIC .*}}

</p>


<script>
document.addEventListener("DOMContentLoaded", () => {
  document.querySelectorAll(".fold-table .summary").forEach(summary => {
    summary.addEventListener("click", () => {
      let row = summary.nextElementSibling;

      while (row && !row.classList.contains("summary")) {
        row.classList.toggle("open");
        row = row.nextElementSibling;
      }
    });
  });
});
</script>
