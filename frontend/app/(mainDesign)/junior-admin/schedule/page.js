"use client";

import { useState, useEffect } from "react";
import { useRouter } from 'next/navigation';
import Link from "next/link";

export default function JuniorAdminSchedulePage() {
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState("");
    const [success, setSuccess] = useState("");
    const router = useRouter();

    // Данные администратора
    const [adminId, setAdminId] = useState(null);
    const [hospitalId, setHospitalId] = useState(null);
    const [hospitalName, setHospitalName] = useState("");

    // Список врачей больницы
    const [doctors, setDoctors] = useState([]);
    const [selectedDoctor, setSelectedDoctor] = useState(null);

    // Состояние для добавления слота
    const [isAddingSlot, setIsAddingSlot] = useState(false);
    const [slotDate, setSlotDate] = useState("");
    const [slotTime, setSlotTime] = useState("");
    const [cabinet, setCabinet] = useState("");

    // Состояние для отображения расписания
    const [schedule, setSchedule] = useState({});
    const [loadingSchedule, setLoadingSchedule] = useState(false);
    const [selectedDate, setSelectedDate] = useState(null);
    const [allDates, setAllDates] = useState([]);

    useEffect(() => {
        // Проверка аутентификации
        const isLoggedIn = localStorage.getItem('isLoggedIn');
        const userData = JSON.parse(localStorage.getItem('medSchedulerUser') || '{}');

        if (!isLoggedIn || !userData.userId || userData.userType !== 'junior') {
            router.push('/login');
            return;
        }

        setAdminId(userData.userId);

        // Получаем ID больницы, привязанной к администратору
        const fetchHospitalInfo = async () => {
            try {
                const response = await fetch(`https://api.medscheduler.ru/get_admin_hospital?admin_id=${userData.userId}`);
                const data = await response.json();

                if (response.ok && data.success && data.hospital) {
                    setHospitalId(data.hospital.hospital_id);
                    setHospitalName(data.hospital.full_name);

                    // После получения информации о больнице загружаем врачей больницы
                    fetchDoctors(data.hospital.hospital_id);
                } else {
                    setError("Не удалось получить информацию о больнице");
                    setLoading(false);
                }
            } catch (err) {
                console.error("Error fetching hospital info:", err);
                setError("Не удалось получить информацию о больнице");
                setLoading(false);
            }
        };

        fetchHospitalInfo();
    }, [router]);

    const fetchDoctors = async (hospitalId) => {
        try {
            // Запрос к API для получения списка врачей больницы
            const response = await fetch(`https://api.medscheduler.ru/get_doctors`);
            const data = await response.json();

            if (response.ok && data.success) {
                // Фильтруем врачей, привязанных к больнице
                // В реальном приложении здесь должен быть отдельный API-метод
                // для получения врачей конкретной больницы
                const doctorsOfHospital = data.doctors.filter(doctor => {
                    // Проверяем, есть ли больница в hospital_ids врача
                    return doctor.hospital_ids &&
                           Array.isArray(doctor.hospital_ids) &&
                           doctor.hospital_ids.includes(hospitalId);
                });

                setDoctors(doctorsOfHospital);
            } else {
                setError(data.error || "Ошибка при получении списка врачей");
            }
        } catch (err) {
            console.error("Error fetching doctors:", err);
            setError("Не удалось загрузить список врачей");
        } finally {
            setLoading(false);
        }
    };

    const fetchSchedule = async (doctorId) => {
        if (!doctorId || !adminId) return;

        setLoadingSchedule(true);
        try {
            const response = await fetch(`https://api.medscheduler.ru/junior_admin_schedule?junior_admin_id=${adminId}&doctor_id=${doctorId}`);
            const data = await response.json();

            if (response.ok && data.success) {
                // Группируем слоты по датам
                const groupedByDate = data.schedule.reduce((acc, slot) => {
                    const date = slot.appointment_date;
                    if (!acc[date]) {
                        acc[date] = [];
                    }
                    acc[date].push(slot);
                    return acc;
                }, {});

                // Сортировка дат
                const sortedDates = Object.keys(groupedByDate).sort();

                setSchedule(groupedByDate);
                setAllDates(sortedDates);

                // Выбираем первую дату по умолчанию, если есть записи
                if (sortedDates.length > 0) {
                    setSelectedDate(sortedDates[0]);
                } else {
                    setSelectedDate(null);
                }
            } else {
                setError(data.error || "Ошибка при получении расписания");
            }
        } catch (err) {
            console.error("Error fetching schedule:", err);
            setError("Не удалось подключиться к серверу");
        } finally {
            setLoadingSchedule(false);
        }
    };

    const handleDoctorChange = (e) => {
        const doctorId = e.target.value;
        if (doctorId) {
            const doctor = doctors.find(d => d.doctor_id === parseInt(doctorId));
            setSelectedDoctor(doctor);
            fetchSchedule(doctorId);
        } else {
            setSelectedDoctor(null);
            setSchedule({});
            setAllDates([]);
            setSelectedDate(null);
        }
    };

    const handleAddSlot = async (e) => {
        e.preventDefault();
        setError("");
        setSuccess("");

        if (!selectedDoctor || !slotDate || !slotTime || !cabinet) {
            setError("Пожалуйста, заполните все поля");
            return;
        }

        try {
            const response = await fetch('https://api.medscheduler.ru/add_record_slot', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({
                    doctor_id: parseInt(selectedDoctor.doctor_id),
                    date: slotDate,
                    time: slotTime + ":00", // Добавляем секунды, если API ожидает формат HH:MM:SS
                    hospital_id: parseInt(hospitalId),
                    cabinet: parseInt(cabinet),
                    junior_admin_id: parseInt(adminId)
                }),
            });

            const data = await response.json();

            if (response.ok && data.success) {
                setSuccess("Слот успешно добавлен");

                // Сбрасываем форму
                setSlotDate("");
                setSlotTime("");
                setCabinet("");

                // Обновляем расписание
                fetchSchedule(selectedDoctor.doctor_id);

                // Скрываем форму через 2 секунды
                setTimeout(() => {
                    setIsAddingSlot(false);
                    setSuccess("");
                }, 10000);
            } else {
                setError(data.error || "Ошибка при добавлении слота");
            }
        } catch (err) {
            console.error("Error adding slot:", err);
            setError("Не удалось подключиться к серверу");
        }
    };

    // Форматирование даты
    const formatDate = (dateStr) => {
        const date = new Date(dateStr);
        return date.toLocaleDateString('ru-RU', {
            day: '2-digit',
            month: '2-digit',
            year: 'numeric',
            weekday: 'long'
        });
    };

    // Форматирование времени
    const formatTime = (timeStr) => {
        return timeStr.substring(0, 5); // Берем только HH:MM из HH:MM:SS
    };

    // Получение минимальной даты для поля ввода даты (сегодня)
    const getMinDate = () => {
        const today = new Date();
        return today.toISOString().split('T')[0];
    };

    if (loading) {
        return (
            <div className="min-h-[calc(80vh-theme(space.36))] flex items-center justify-center">
                <div className="text-xl text-gray-600">Загрузка данных...</div>
            </div>
        );
    }

    return (
        <div className="max-w-6xl mx-auto py-8 px-4">
            <h1 className="text-3xl font-bold text-main2 mb-8">Управление расписанием</h1>

            {error && (
                <div className="bg-red-100 border border-red-400 text-red-700 px-4 py-3 rounded mb-4">
                    {error}
                </div>
            )}

            {success && (
                <div className="bg-green-100 border border-green-400 text-green-700 px-4 py-3 rounded mb-4">
                    {success}
                </div>
            )}

            <div className="bg-white shadow-md rounded-lg p-6 mb-6">
                <div className="flex flex-col md:flex-row justify-between md:items-center mb-6">
                    <div>
                        <h2 className="text-xl font-semibold text-main2">Ваша больница</h2>
                        <p className="text-gray-600 mt-1">{hospitalName || "Больница не привязана"}</p>
                    </div>
                </div>

                <div className="mb-6">
                    <label htmlFor="doctorSelect" className="block text-sm font-medium text-gray-700 mb-2">
                        Выберите врача
                    </label>
                    <select
                        id="doctorSelect"
                        onChange={handleDoctorChange}
                        className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                        defaultValue=""
                    >
                        <option value="" disabled>Выберите врача</option>
                        {doctors.map((doctor) => (
                            <option key={doctor.doctor_id} value={doctor.doctor_id}>
                                {doctor.last_name} {doctor.first_name} {doctor.patronymic} - {doctor.specialty}
                            </option>
                        ))}
                    </select>
                </div>

                {selectedDoctor && (
                    <div className="flex justify-end mb-4">
                        <button
                            onClick={() => setIsAddingSlot(true)}
                            className="px-4 py-2 bg-main text-white rounded hover:bg-main2 transition-colors"
                        >
                            Добавить слот
                        </button>
                    </div>
                )}

                {isAddingSlot && selectedDoctor && (
                    <div className="mb-6 p-4 bg-gray-50 rounded-lg">
                        <h3 className="text-lg font-medium text-gray-800 mb-4">
                            Добавить слот для записи
                        </h3>
                        <form onSubmit={handleAddSlot}>
                            <div className="grid grid-cols-1 md:grid-cols-3 gap-4 mb-4">
                                <div>
                                    <label htmlFor="slotDate" className="block text-sm font-medium text-gray-700 mb-1">
                                        Дата *
                                    </label>
                                    <input
                                        type="date"
                                        id="slotDate"
                                        value={slotDate}
                                        min={getMinDate()}
                                        onChange={(e) => setSlotDate(e.target.value)}
                                        className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                        required
                                    />
                                </div>
                                <div>
                                    <label htmlFor="slotTime" className="block text-sm font-medium text-gray-700 mb-1">
                                        Время *
                                    </label>
                                    <input
                                        type="time"
                                        id="slotTime"
                                        value={slotTime}
                                        onChange={(e) => setSlotTime(e.target.value)}
                                        className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                        required
                                    />
                                </div>
                                <div>
                                    <label htmlFor="cabinet" className="block text-sm font-medium text-gray-700 mb-1">
                                        Кабинет *
                                    </label>
                                    <input
                                        type="number"
                                        id="cabinet"
                                        value={cabinet}
                                        onChange={(e) => setCabinet(e.target.value)}
                                        min="1"
                                        className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                        required
                                    />
                                </div>
                            </div>
                            <div className="mb-3">
                                <p className="text-sm text-gray-700">Врач: <span className="font-medium">{selectedDoctor.last_name} {selectedDoctor.first_name} {selectedDoctor.patronymic}</span></p>
                                <p className="text-sm text-gray-700">Больница: <span className="font-medium">{hospitalName}</span></p>
                            </div>
                            <div className="flex gap-3">
                                <button
                                    type="submit"
                                    className="px-4 py-2 bg-main text-white rounded hover:bg-main2 transition-colors"
                                >
                                    Добавить слот
                                </button>
                                <button
                                    type="button"
                                    onClick={() => setIsAddingSlot(false)}
                                    className="px-4 py-2 bg-gray-200 text-gray-800 rounded hover:bg-gray-300 transition-colors"
                                >
                                    Отмена
                                </button>
                            </div>
                        </form>
                    </div>
                )}

                {selectedDoctor && (
                    <div>
                        <h3 className="text-lg font-medium text-gray-800 mb-4">
                            Расписание {selectedDoctor.last_name} {selectedDoctor.first_name} {selectedDoctor.patronymic}
                        </h3>

                        {loadingSchedule ? (
                            <div className="text-center py-6">
                                <p className="text-gray-600">Загрузка расписания...</p>
                            </div>
                        ) : allDates.length === 0 ? (
                            <div className="text-center py-6 bg-gray-50 rounded-lg">
                                <p className="text-gray-600">У врача нет запланированных приемов</p>
                            </div>
                        ) : (
                            <div className="grid grid-cols-1 md:grid-cols-4 gap-6">
                                {/* Выбор даты - боковая панель */}
                                <div className="md:col-span-1">
                                    <div className="bg-gray-50 rounded-lg p-4">
                                        <h4 className="text-md font-medium text-gray-700 mb-3">Выберите дату</h4>
                                        <div className="space-y-2">
                                            {allDates.map((date) => (
                                                <button
                                                    key={date}
                                                    onClick={() => setSelectedDate(date)}
                                                    className={`w-full text-left px-3 py-2 rounded-md transition-colors ${
                                                        selectedDate === date
                                                        ? "bg-main text-white"
                                                        : "bg-white text-gray-700 hover:bg-gray-100"
                                                    }`}
                                                >
                                                    {formatDate(date)}
                                                    <span className="block text-xs mt-1">
                                                        {schedule[date].length} {schedule[date].length === 1 ? "запись" :
                                                        schedule[date].length > 1 && schedule[date].length < 5 ? "записи" : "записей"}
                                                    </span>
                                                </button>
                                            ))}
                                        </div>
                                    </div>
                                </div>

                                {/* Расписание на выбранную дату */}
                                <div className="md:col-span-3">
                                    {selectedDate ? (
                                        <div className="bg-white border border-gray-200 rounded-lg overflow-hidden">
                                            <div className="px-6 py-4 bg-gray-50 border-b border-gray-200">
                                                <h4 className="text-lg font-medium text-gray-800">
                                                    {formatDate(selectedDate)}
                                                </h4>
                                            </div>

                                            <table className="min-w-full divide-y divide-gray-200">
                                                <thead className="bg-gray-50">
                                                    <tr>
                                                        <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                                            Время
                                                        </th>
                                                        <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                                            Статус
                                                        </th>
                                                        <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                                            Кабинет
                                                        </th>
                                                    </tr>
                                                </thead>
                                                <tbody className="bg-white divide-y divide-gray-200">
                                                    {schedule[selectedDate].sort((a, b) => a.appointment_time.localeCompare(b.appointment_time)).map((slot) => (
                                                        <tr key={`${slot.appointment_date}-${slot.appointment_time}-${slot.cabinet}`}>
                                                            <td className="px-6 py-4 whitespace-nowrap">
                                                                <div className="text-sm font-medium text-gray-900">
                                                                    {formatTime(slot.appointment_time)}
                                                                </div>
                                                            </td>
                                                            <td className="px-6 py-4 whitespace-nowrap">
                                                                {slot.patient_name ? (
                                                                    <span className="px-2 inline-flex text-xs leading-5 font-semibold rounded-full bg-red-100 text-red-800">
                                                                        Занято
                                                                    </span>
                                                                ) : (
                                                                    <span className="px-2 inline-flex text-xs leading-5 font-semibold rounded-full bg-green-100 text-green-800">
                                                                        Свободно
                                                                    </span>
                                                                )}
                                                            </td>
                                                            <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-500">
                                                                {slot.cabinet}
                                                            </td>
                                                        </tr>
                                                    ))}
                                                </tbody>
                                            </table>
                                        </div>
                                    ) : (
                                        <div className="bg-white border border-gray-200 rounded-lg p-6 text-center">
                                            <p className="text-gray-600">Выберите дату для просмотра расписания</p>
                                        </div>
                                    )}
                                </div>
                            </div>
                        )}
                    </div>
                )}
            </div>

            <div className="flex justify-between">
                <Link href="/junior-admin/doctors" className="text-main hover:text-main2 transition-colors">
                    ← Управление врачами
                </Link>
                <Link href="/junior-admin/waitlist" className="text-main hover:text-main2 transition-colors">
                    Лист ожидания →
                </Link>
            </div>
        </div>
    );
}
